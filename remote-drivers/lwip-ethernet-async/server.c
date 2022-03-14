/*
 * Copyright 2020, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(DATA61_GPL)
 */

#include <autoconf.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <camkes/dma.h>
#include <camkes/dataport.h>
#include <camkes/io.h>
#include <camkes/irq.h>

#include <platsupport/io.h>
#include <platsupport/irq.h>
#include <platsupport/interface_registration.h>
#include <ethdrivers/raw.h>
#include <ethdrivers/intel.h>
#include <sel4utils/sel4_zf_logif.h>
#include <lwip-ethernet-async.h>

#include <utils/util.h>

#include <shared_ringbuffer/shared_ringbuffer.h>
#include <shared_ringbuffer/gen_config.h>

#define BUF_SIZE 2048

typedef struct data {
    struct eth_driver *eth_driver;
    /* mac address */
    uint8_t hw_mac[6];
    ps_io_ops_t *io_ops;

    /* Pointers to shared_ringbuffers */
    ring_handle_t *rx_ring;
    ring_handle_t *tx_ring;
} server_data_t;

/* The camkes notification connection unregisters the callback each time. */
static register_cb_fn reg_tx_cb;

/**
 * Packets have been transferred or dropped. Return the buffer
 * into the available queue for reuse. 
 * This is called by the actual driver.
 *
 * @param iface server data.
 * @param cookie pointer to bufer descriptor to move.
 *
 */
static void eth_tx_complete(void *iface, void *cookie)
{   
    trace_extra_point_start(1);
    ZF_LOGW("Packets have been transferred or dropped");
    server_data_t *state = iface;

    buff_desc_t *desc = cookie;

    int err = enqueue_avail(state->tx_ring, desc->encoded_addr, desc->len, desc->cookie);
    ZF_LOGF_IF(err, "lwip_eth_send: Error while enqueuing available buffer, tx available queue full");

    trace_extra_point_end(1, 1);
}

/**
 * Fetch an available buffer for receiving packets.   
 * This is called by the actual driver.
 *
 * @param iface server data.
 * @param buf_size size of buffer required. 
 * @param cookie pointer to bufer descriptor to move.
 *
 * @return the physical memory address of the buffer. 
 */
static uintptr_t eth_allocate_rx_buf(void *iface, size_t buf_size, void **cookie)
{
    if (buf_size > BUF_SIZE) {
        return 0;
    }
    server_data_t *state = iface;

    uintptr_t addr;
    size_t len;

    /* Try to grab a buffer from the available ring */
    if (driver_dequeue(state->rx_ring->avail_ring, &addr, &len, cookie)) {
        ZF_LOGE("RX Available ring is empty. No more buffers available");
        return 0;
    }

    void *decoded_buf = DECODE_DMA_ADDRESS(addr);
    ZF_LOGF_IF(decoded_buf == NULL, "Decoded DMA buffer is NULL");

    /* Invalidate the memory */
    ps_dma_cache_invalidate(&state->io_ops->dma_manager, decoded_buf, buf_size);
    uintptr_t phys = ps_dma_pin(&state->io_ops->dma_manager, decoded_buf, buf_size);

    ZF_LOGW("phys: %p", phys);

    return phys;
}

/**
 * Packets have been received and are ready for processing. 
 * Enqueue the packets into shared memory and notify the network stack. 
 *
 * @param iface server data.
 * @param num_bufs the number of buffers filled with data.
 * @param cookies an array of length num_bufs containing cookies passed in by eth_allocate_rx_buf()
 * @param lens an array on length num_bufs containing the length of data each buffer now contains.  
 *
 */
static void eth_rx_complete(void *iface, unsigned int num_bufs, void **cookies, unsigned int *lens)
{
    trace_extra_point_start(0);
    server_data_t *state = iface;
    
    for (int i = 0; i < num_bufs; i++) {
        /* Add buffers to used rx ring. */
        buff_desc_t *desc = cookies[i];
        int err = enqueue_used(state->rx_ring, desc->encoded_addr, (size_t)lens[i], desc->cookie);
    }

    /* Notify the client */
    notify(state->rx_ring);
    trace_extra_point_end(0, 1);
}

static struct raw_iface_callbacks ethdriver_callbacks = {
    .tx_complete = eth_tx_complete,
    .rx_complete = eth_rx_complete,
    .allocate_rx_buf = eth_allocate_rx_buf
};

/**
 * This function is the notification handler when packets are ready for transmitting.
 * Dequeue the buffers from shared memory and pass them to the driver. 
 *
 * @param iface server data.
 *
 */
static void tx_send(void *iface)
{
    trace_extra_point_start(2);
    ZF_LOGW("We have packets that need to be sent");
    server_data_t *state = iface;

    uintptr_t buffer;
    size_t len;
    void *cookie;

    while(!driver_dequeue(state->tx_ring->used_ring, &buffer, &len, &cookie)) {
        ZF_LOGW("We are sending packet %p of length %d", buffer, len);
        void *decoded_buf = DECODE_DMA_ADDRESS(buffer);
        ZF_LOGF_IF(decoded_buf == NULL, "Decoded DMA buffer is NULL");

        uintptr_t phys = ps_dma_pin(&state->io_ops->dma_manager, decoded_buf, len);
        ps_dma_cache_clean(&state->io_ops->dma_manager, decoded_buf, len);

        int err = state->eth_driver->i_fn.raw_tx(state->eth_driver, 1, &phys, &len, cookie);
        if (err != ETHIF_TX_ENQUEUED) {
            eth_tx_complete(state, cookie);
        }
    }

    int error = reg_tx_cb(tx_send, state);
    ZF_LOGF_IF(error, "Unable to register transmit callback handler");
    trace_extra_point_end(2, 1);
}

/**
 * This function is called via IPC for the client to obtain the MAC address. 
 *
 * @param b1, b2, b3, b4, b5, b6 pointers to 6 bytes used for storing the MAC address. 
 * @param cookie server data.
 */
static void client_get_mac(uint8_t *b1, uint8_t *b2, uint8_t *b3, uint8_t *b4, uint8_t *b5, uint8_t *b6, void *cookie)
{
    server_data_t *state = cookie;
    *b1 = state->hw_mac[0];
    *b2 = state->hw_mac[1];
    *b3 = state->hw_mac[2];
    *b4 = state->hw_mac[3];
    *b5 = state->hw_mac[4];
    *b6 = state->hw_mac[5];
}

/**
 * Provides an interface for the hardware. 
 *
 * @param cookie server data.
 */
static int hardware_interface_searcher(void *cookie, void *interface_instance, char **properties)
{
    server_data_t *state = cookie;
    state->eth_driver = interface_instance;
    return PS_INTERFACE_FOUND_MATCH;
}

/**
 * Initialise the transmit shared ring buffers and registers the callback handler for 
 * the transmit notification
 *
 * @param state server data.
 * @param tx_avail pointer to shared memory region to be initialised as the available ring buffer.
 * @param tx_used pointer to shared memory region to be initialised as the used ring buffer.
 * @param reg_tx function pointer to register a callback handler. 
 */
static void server_init_tx(server_data_t *state, void *tx_avail, void *tx_used, register_cb_fn reg_tx)
{
    int error = reg_tx(tx_send, state);
    if (error) {
        ZF_LOGE("Unable to register handler");
    }

    ring_init(state->tx_ring, tx_avail, tx_used, NULL, 0);
    
    reg_tx_cb = reg_tx;
}

/**
 * Initialise the receive shared ring buffers. 
 *
 * @param state server data.
 * @param rx_avail pointer to shared memory region to be initialised as the available ring buffer.
 * @param rx_used pointer to shared memory region to be initialised as the used ring buffer.
 * @param rx_notify function pointer to notify the client side on the receive path. 
 */
static void server_init_rx(server_data_t *state, void *rx_avail, void *rx_used, notify_fn rx_notify)
{
    ring_init(state->rx_ring, rx_avail, rx_used, rx_notify, 0);
}

/**
 * Initialise the server side of the lwip ethernet interface. 
 *
 * @param io_ops data structure containing I/O functions. 
 * @param register_get_mac_fn function pointer to register a function for the client to obtain the MAC address. 
 * @param rx_avail pointer to shared memory region to be initialised as the available ring buffer.
 * @param rx_used pointer to shared memory region to be initialised as the used ring buffer.
 * @param tx_avail pointer to shared memory region to be initialised as the available ring buffer.
 * @param tx_used pointer to shared memory region to be initialised as the used ring buffer.
 * @param reg_tx function pointer to register a callback handler. 
 * @param rx_notify function pointer to notify the client side on the receive path. 
 */
int lwip_ethernet_async_server_init(ps_io_ops_t *io_ops, register_get_mac_server_fn register_get_mac_fn,
                void *rx_avail, void *rx_used, void *tx_avail, void *tx_used, register_cb_fn reg_tx_cb, 
                notify_fn rx_notify)
{
    server_data_t *data;
    int error = ps_calloc(&io_ops->malloc_ops, 1, sizeof(*data), (void **)&data);
    ZF_LOGF_IF(error, "Failed to calloc server data");
    data->io_ops = io_ops;

    ring_handle_t *rx_ring;
    error = ps_calloc(&io_ops->malloc_ops, 1, sizeof(*rx_ring), (void **)&rx_ring);
    ZF_LOGF_IF(error, "Failed to calloc rx ring data");
    data->rx_ring = rx_ring;
    server_init_rx(data, rx_avail, rx_used, rx_notify);

    ring_handle_t *tx_ring;
    error = ps_calloc(&io_ops->malloc_ops, 1, sizeof(*tx_ring), (void **)&tx_ring);
    ZF_LOGF_IF(error, "Failed to calloc tx ring data");
    data->tx_ring = tx_ring;
    server_init_tx(data, tx_avail, tx_used, reg_tx_cb);


    error = ps_interface_find(&io_ops->interface_registration_ops,
                              PS_ETHERNET_INTERFACE, hardware_interface_searcher, data);
    if (error) {
        ZF_LOGF("Unable to find an ethernet device");
    }

    data->eth_driver->cb_cookie = data;
    data->eth_driver->i_cb = ethdriver_callbacks;

    error = trace_extra_point_register_name(0, "eth_rx_complete");
    ZF_LOGF_IF(error, "Failed to register extra trace point 0");
    error = trace_extra_point_register_name(1, "eth_tx_complete");
    ZF_LOGF_IF(error, "Failed to register extra trace point 1");
    error = trace_extra_point_register_name(2, "tx_send ntfn");
    ZF_LOGF_IF(error, "Failed to register extra trace point 2");

    data->eth_driver->i_fn.get_mac(data->eth_driver, data->hw_mac);
    data->eth_driver->i_fn.raw_poll(data->eth_driver);

    register_get_mac_fn(client_get_mac, data);
    return 0;
}
