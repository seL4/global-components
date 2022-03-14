/*
 * Copyright 2020, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */

#include <autoconf.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <sel4/sel4.h>


#include <camkes/dataport.h>
#include <lwip-ethernet-async.h>

#include <lwip/init.h>
#include <netif/etharp.h>
#include <lwip/pbuf.h>
#include <lwip/netif.h>
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include <lwip/sys.h>

#include <shared_ringbuffer/shared_ringbuffer.h>

#define LINK_SPEED 1000000000 // Gigabit
#define ETHER_MTU 1500
#define NUM_BUFFERS 512
#define BUF_SIZE 2048
/*
 * These structures track the buffers used to construct packets
 * sent via this network interface.
 */
typedef enum {
    ORIGIN_RX_QUEUE,
    ORIGIN_TX_QUEUE,
} ethernet_buffer_origin_t;

typedef struct ethernet_buffer {
    /* The acutal underlying memory of the buffer */
    unsigned char *buffer;
    /* The encoded DMA address */
    uintptr_t dma_addr;
    /* The physical size of the buffer */
    size_t size;
    /* Queue from which the buffer was allocated */
    char origin;
    /* Index into buffer_metadata array */
    unsigned int index;
} ethernet_buffer_t;

typedef struct state {
    struct netif netif;
    /* mac address for this client */
    uint8_t mac[6];
    ps_io_ops_t *io_ops;

    /* Pointers to shared buffers */
    ring_handle_t *rx_ring;
    ring_handle_t *tx_ring;
    /*
     * Metadata associated with buffers
     */
    ethernet_buffer_t buffer_metadata[NUM_BUFFERS * 2];
} state_t;

static register_cb_fn reg_rx_cb;

/**
 * Buffers are available and can be reused. 
 * Return them to shared memory. 
 *
 * @param state client state data. 
 * @param buffer ethernet buffer to return. 
 *
 */
static inline void return_buffer(state_t *state, ethernet_buffer_t *buffer)
{
    assert(buffer->origin == ORIGIN_RX_QUEUE);
    ZF_LOGW("Returning RX buffer");
    /* As the rx_available ring is the size of the number of buffers we have, 
    the ring should never be full. */
    enqueue_avail(state->rx_ring, buffer->dma_addr, BUF_SIZE, buffer);
}

/**
 * Allocate an empty TX buffer from the empty pool
 *
 * @param state client state data. 
 * @param length length of buffer required 
 *
 */
static inline ethernet_buffer_t *alloc_tx_buffer(state_t *state, size_t length)
{   
    if (BUF_SIZE < length) {
        ZF_LOGE("Requested buffer size too large.");
        return NULL;
    }

    uintptr_t encoded_addr;
    size_t len;
    ethernet_buffer_t *buffer;

    int err = dequeue_avail(state->tx_ring, &encoded_addr, &len, (void **)&buffer);
    ZF_LOGF_IF(err, "tx available ring is empty");

    /* Little sanity check */
    if (buffer->dma_addr != encoded_addr) {
        ZF_LOGE("buffer->dma_addr != encoded_addr, dma_addr = %p, encoded_addr = %p", buffer->dma_addr, encoded_addr);
    }

    return buffer;
}

typedef struct lwip_custom_pbuf {
    struct pbuf_custom custom;
    ethernet_buffer_t *buffer;
    state_t *state;
} lwip_custom_pbuf_t;
LWIP_MEMPOOL_DECLARE(
    RX_POOL,
    NUM_BUFFERS * 2,
    sizeof(lwip_custom_pbuf_t),
    "Zero-copy RX pool"
);

/**
 * Free a pbuf. This also returns the underlying buffer to
 * the appropriate place. 
 *
 * @param buf pbuf to free. 
 *
 */
static void interface_free_buffer(struct pbuf *buf)
{
    SYS_ARCH_DECL_PROTECT(old_level);

    lwip_custom_pbuf_t *custom_pbuf = (lwip_custom_pbuf_t *) buf;

    SYS_ARCH_PROTECT(old_level);
    return_buffer(custom_pbuf->state, custom_pbuf->buffer);
    LWIP_MEMPOOL_FREE(RX_POOL, custom_pbuf);
    SYS_ARCH_UNPROTECT(old_level);
}

/**
 * Create a pbuf structure to pass to the network interface. 
 *
 * @param state client state data. 
 * @param buffer ethernet buffer containing metadata for the actual buffer
 * @param length length of data
 * 
 * @return the newly created pbuf. 
 */
static struct pbuf *create_interface_buffer(state_t *state, ethernet_buffer_t *buffer, size_t length) 
{
    lwip_custom_pbuf_t *custom_pbuf = (lwip_custom_pbuf_t *) LWIP_MEMPOOL_ALLOC(RX_POOL);

    custom_pbuf->state = state;
    custom_pbuf->buffer = buffer;
    custom_pbuf->custom.custom_free_function = interface_free_buffer;

    return pbuf_alloced_custom(
        PBUF_RAW,
        length,
        PBUF_REF,
        &custom_pbuf->custom,
        buffer->buffer,
        buffer->size
    );
}

/**
 * Packets have been received and are ready for processing. 
 * Dequeue the packets from shared memory and pass them to lwip to proess. 
 *
 * @param cookie client state data. 
 *
 */
static void rx_queue(void *cookie)
{
    trace_extra_point_start(0);
    ZF_LOGW("New packets have been received");
    state_t *state = cookie;
    /* get buffers from used RX ring */
    while(!ring_empty(state->rx_ring->used_ring)) {
        uintptr_t encoded_addr;
        size_t len;
        ethernet_buffer_t *buffer;

        dequeue_used(state->rx_ring, &encoded_addr, &len, (void **)&buffer);

        /* Little sanity check */
        if (buffer->dma_addr != encoded_addr) {
            ZF_LOGE("buffer->dma_addr != encoded_addr, dma_addr = %p, encoded_addr = %p", buffer->dma_addr, encoded_addr);
        }

        ZF_LOGW("processing packet %p, index %d, of length %d", buffer->dma_addr, buffer->index, len);

        struct pbuf *p = create_interface_buffer(state, buffer, len);

        if (state->netif.input(p, &state->netif) != ERR_OK) {
            // If it is successfully received, the receiver controls whether or not it gets freed.
            ZF_LOGE("netif.input() != ERR_OK");
            pbuf_free(p);
        }
    }

    int error = reg_rx_cb(rx_queue, state);
    ZF_LOGF_IF(error, "Unable to register handler");

    trace_extra_point_end(0, 1);
}


/* We have packets to send */
/**
 * Packets are ready to be transmitted.  
 * Enqueue the packets into shared memory and notify the driver. 
 *
 * @param netif network interface data.
 * @param p lwip defined buffer containing the packet to send. 
 *
 */
static err_t lwip_eth_send(struct netif *netif, struct pbuf *p)
{
    /* Grab an available TX buffer, copy pbuf data over, 
    add to used tx ring, notify server */
    err_t ret = ERR_OK;

    ZF_LOGW("We have packets to send from lwip");

    if (p->tot_len > BUF_SIZE) {
        ZF_LOGF("len %hu is invalid in lwip_eth_send", p->tot_len);
        return ERR_MEM;
    }

    state_t *state = (state_t *)netif->state;

    ethernet_buffer_t *buffer = alloc_tx_buffer(state, p->tot_len);
    if (buffer == NULL) {
        ZF_LOGF("Out of ethernet memory");
        return ERR_MEM;
    }
    unsigned char *frame = buffer->buffer;

    /* Copy all buffers that need to be copied */
    unsigned int copied = 0;
    for (struct pbuf *curr = p; curr != NULL; curr = curr->next) {
        unsigned char *buffer_dest = &frame[copied];
        if ((uintptr_t)buffer_dest != (uintptr_t)curr->payload) {
            /* Don't copy memory back into the same location */
            memcpy(buffer_dest, curr->payload, curr->len);
        }
        copied += curr->len;
    }

    /* insert into the used tx queue */
    int error = enqueue_used(state->tx_ring, (void *)ENCODE_DMA_ADDRESS(frame), copied, buffer);
    if (error) {
        ZF_LOGF("lwip_eth_send: Error while enqueuing used buffer, tx used queue full");
        return_buffer(state, buffer);
        return ERR_MEM;
    }

    /* Notify the server */
    notify(state->tx_ring);

    return ret;
}

/**
 * Initialise the network interface data structure. 
 *
 * @param netif network interface data structuer.
 */
static err_t ethernet_init(struct netif *netif)
{
    if (netif->state == NULL) {
        return ERR_ARG;
    }

    state_t *data = netif->state;

    netif->hwaddr[0] = data->mac[0];
    netif->hwaddr[1] = data->mac[1];
    netif->hwaddr[2] = data->mac[2];
    netif->hwaddr[3] = data->mac[3];
    netif->hwaddr[4] = data->mac[4];
    netif->hwaddr[5] = data->mac[5];
    netif->mtu = ETHER_MTU;
    netif->hwaddr_len = ETHARP_HWADDR_LEN;
    netif->output = etharp_output;
    netif->linkoutput = lwip_eth_send;
    NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED);
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP | NETIF_FLAG_IGMP;

    ZF_LOGW("Ethernet initialised");
    return ERR_OK;
}

/**
 * Initialise the transmit shared ring buffers
 *
 * @param state server data.
 * @param tx_avail pointer to shared memory region to be initialised as the available ring buffer.
 * @param tx_used pointer to shared memory region to be initialised as the used ring buffer.
 * @param reg_tx function pointer to register a callback handler. 
 */
static void client_init_tx(state_t *data, void *tx_avail, void *tx_used, notify_fn tx_notify)
{
    ring_init(data->tx_ring, tx_avail, tx_used, tx_notify, 1);

    /* Allocate tx rings */
    for (int i = 0; i < NUM_BUFFERS - 1; i++) {
        void *buf = ps_dma_alloc(
            &data->io_ops->dma_manager,
            BUF_SIZE,
            64, /* Align */
            1, /* Mapped cached */
            PS_MEM_NORMAL
        );
        ZF_LOGF_IF(!buf, "Failed to allocate buffer for pending TX ring.");
        memset(buf, 0, BUF_SIZE);

        ethernet_buffer_t *buffer = &data->buffer_metadata[i + NUM_BUFFERS];
        *buffer = (ethernet_buffer_t) {
            .buffer = buf,
            .dma_addr = (void *)ENCODE_DMA_ADDRESS(buf),
            .size = BUF_SIZE,
            .origin = ORIGIN_TX_QUEUE,
            .index = i + NUM_BUFFERS,
        };

        enqueue_avail(data->tx_ring, buffer->dma_addr, BUF_SIZE, buffer);
    }
}

/**
 * Initialise the receive shared ring buffers and registers the callback handler for 
 * the receive notification
 * 
 * @param data client data.
 * @param rx_avail pointer to shared memory region to be initialised as the available ring buffer.
 * @param rx_used pointer to shared memory region to be initialised as the used ring buffer.
 * @param reg_rx function pointer to register a callback handler. 
 */
static void client_init_rx(state_t *data, void *rx_avail, void *rx_used, register_cb_fn reg_rx)
{
    int error = reg_rx(rx_queue, data);
    if (error) {
        ZF_LOGE("Unable to register handler");
    }

    reg_rx_cb = reg_rx;

    ring_init(data->rx_ring, rx_avail, rx_used, NULL, 1);

    /* Pre allocate buffers */  
    for (int i = 0; i < NUM_BUFFERS - 1; i++) {
        void *buf = ps_dma_alloc(
            &data->io_ops->dma_manager,
            BUF_SIZE,
            64,
            1,
            PS_MEM_NORMAL
        );
        assert(buf);
        memset(buf, 0, BUF_SIZE);
        ZF_LOGF_IF(buf == NULL, "Failed to allocate DMA memory for pending rx ring");

        ethernet_buffer_t *buffer = &data->buffer_metadata[i];

        *buffer = (ethernet_buffer_t) {
            .buffer = buf,
            .dma_addr = (void *)ENCODE_DMA_ADDRESS(buf),
            .size = BUF_SIZE,
            .origin = ORIGIN_RX_QUEUE,
            .index = i,
        };

        enqueue_avail(data->rx_ring, buffer->dma_addr, BUF_SIZE, buffer);
    }

}

/**
 * Initialise the client side of the lwip ethernet interface. 
 * It also allocates the shared memory regions for DMA. 
 *
 * @param io_ops data structure containing I/O functions. 
 * @param get_mac_fn function pointer to obtain the MAC address from the server. 
 * @param rx_avail pointer to shared memory region to be initialised as the available ring buffer.
 * @param rx_used pointer to shared memory region to be initialised as the used ring buffer.
 * @param tx_avail pointer to shared memory region to be initialised as the available ring buffer.
 * @param tx_used pointer to shared memory region to be initialised as the used ring buffer.
 * @param reg_rx_cb function pointer to register a callback handler. 
 * @param tx_notify function pointer to notify the server side on the transmit path. 
 */
int lwip_ethernet_async_client_init(ps_io_ops_t *io_ops, get_mac_client_fn_t get_mac,
                void *rx_avail, void *rx_used, void *tx_avail, void *tx_used, 
                register_cb_fn reg_rx_cb, notify_fn tx_notify)
{
    state_t *data;
    int error = ps_calloc(&io_ops->malloc_ops, 1, sizeof(*data), (void **)&data);
    ZF_LOGF_IF(error != 0, "Unable to ethernet state");
    data->io_ops = io_ops;

    ring_handle_t *rx_ring;
    error = ps_calloc(&io_ops->malloc_ops, 1, sizeof(*rx_ring), (void **)&rx_ring);
    ZF_LOGF_IF(error, "Failed to calloc rx ring data");
    data->rx_ring = rx_ring;
    client_init_rx(data, rx_avail, rx_used, reg_rx_cb);

    ring_handle_t *tx_ring;
    error = ps_calloc(&io_ops->malloc_ops, 1, sizeof(*tx_ring), (void **)&tx_ring);
    ZF_LOGF_IF(error, "Failed to calloc tx ring data");
    data->tx_ring = tx_ring;
    client_init_tx(data, tx_avail, tx_used, tx_notify);

    error = trace_extra_point_register_name(0, "rx_queue ntfn");
    ZF_LOGF_IF(error, "Failed to register extra trace point 0");

    LWIP_MEMPOOL_INIT(RX_POOL);

    get_mac(&data->mac[0], &data->mac[1], &data->mac[2], &data->mac[3], &data->mac[4], &data->mac[5]);

    /* Set some dummy IP configuration values to get lwIP bootstrapped  */
    struct ip4_addr netmask, ipaddr, gw, multicast;
    ipaddr_aton("0.0.0.0", &gw);
    ipaddr_aton("0.0.0.0", &ipaddr);
    ipaddr_aton("0.0.0.0", &multicast);
    ipaddr_aton("255.255.255.0", &netmask);

    data->netif.name[0] = 'e';
    data->netif.name[1] = '0';

    if (!netif_add(&data->netif, &ipaddr, &netmask, &gw, data,
              ethernet_init, ethernet_input)) {
        ZF_LOGE("Netif add returned NULL");
    }
    netif_set_default(&data->netif);

    return 0;    
}