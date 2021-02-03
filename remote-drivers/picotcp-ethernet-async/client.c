/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <autoconf.h>

#include <string.h>
#include <sel4/sel4.h>

#include <virtqueue.h>
#include <camkes/virtqueue.h>
#include <camkes/dataport.h>

#include <picotcp-ethernet-async.h>

#ifdef PACKED
#undef PACKED
#endif
/* PACKED is redefined by picotcp */
#include <pico_stack.h>
#include <pico_device.h>


#define NUM_TX_BUFS 254
#define NUM_RX_BUFS 254
#define BUF_SIZE 2048

typedef struct state {
    struct pico_device pico_dev;

    /* keeps track of how many TX buffers are in use */
    int num_tx;
    /*
     * this represents the pool of buffers that can be used for TX,
     * this array is a sliding array in that num_tx acts a pointer to
     * separate between buffers that are in use and buffers that are
     * not in use. E.g. 'o' = free, 'x' = in use
     *  -------------------------------------
     *  | o | o | o | o | o | o | x | x | x |
     *  -------------------------------------
     *                          ^
     *                        num_tx
     */
    void *pending_tx[NUM_TX_BUFS];

    /* mac address for this client */
    uint8_t mac[6];
    virtqueue_driver_t tx_virtqueue;
    virtqueue_driver_t rx_virtqueue;
    ps_io_ops_t *io_ops;
    bool action;
} state_t;

struct id_state_mapper {
    uint32_t dataport_id;
    state_t *state;
};

static struct id_state_mapper *mapper;
static size_t mapper_len;

/* 1500 is the standard ethernet MTU at the network layer. */
#define ETHER_MTU 1500


static int pico_eth_send(struct pico_device *dev, void *input_buf, int len)
{
    state_t *state = (state_t *)dev;

    virtqueue_ring_object_t handle;
    uint32_t sent_len;
    void *buf;
    if (virtqueue_get_used_buf(&state->tx_virtqueue, &handle, &sent_len) == 0) {
        if (state->num_tx != 0) {
            state->num_tx --;
            buf = state->pending_tx[state->num_tx];
        } else {
            // No free packets to use.
            return 0;
        }
    } else {
        vq_flags_t flag;
        int more = virtqueue_gather_used(&state->tx_virtqueue, &handle, &buf, &sent_len, &flag);
        if (more == 0) {
            ZF_LOGF("pico_eth_send: Invalid virtqueue ring entry");
        }

        buf = DECODE_DMA_ADDRESS(buf);
    }

    /* copy the packet over */
    memcpy(buf, input_buf, len);
    ps_dma_cache_clean(&state->io_ops->dma_manager, buf, len);

    virtqueue_init_ring_object(&handle);
    if (!virtqueue_add_available_buf(&state->tx_virtqueue, &handle, ENCODE_DMA_ADDRESS(buf), len, VQ_RW)) {
        ZF_LOGF("pico_eth_send: Error while enqueuing available buffer, queue full");
    }
    state->action = true;
    return len;
}


static void pico_free_buf(uint8_t *buf)
{

    /* Pico doesn't allow callbacks to be registered with cookies...
     * We rely on the fact that the buffer was allocated from a dataport and use
     * the dataport ID to find the driver state from a global mapper array.
     */
    dataport_ptr_t wrapped_ptr = dataport_wrap_ptr(buf);
    state_t *state = NULL;
    for (int i = 0; i < mapper_len; i++) {
        if (mapper[i].dataport_id == wrapped_ptr.id) {
            state = mapper[i].state;
            break;
        }
    }

    virtqueue_ring_object_t handle;
    virtqueue_init_ring_object(&handle);
    if (!virtqueue_add_available_buf(&state->rx_virtqueue, &handle, ENCODE_DMA_ADDRESS(buf), BUF_SIZE, VQ_RW)) {
        ZF_LOGF("Error while enqueuing available buffer");
    }
}

/* Async driver will set a flag to signal that there is work to be done  */
static int pico_eth_poll(struct pico_device *dev, int loop_score)
{
    state_t *state = (state_t *)dev;
    while (loop_score > 0) {
        virtqueue_ring_object_t handle;

        uint32_t len;

        unsigned next = (state->rx_virtqueue.u_ring_last_seen + 1) & (state->rx_virtqueue.queue_len - 1);

        if (next == state->rx_virtqueue.used_ring->idx) {
            break;
        }

        handle.first = state->rx_virtqueue.used_ring->ring[next].id;
        len = state->rx_virtqueue.used_ring->ring[next].len;

        handle.cur = handle.first;

        void *buf;
        vq_flags_t flag;
        int more = virtqueue_gather_used(&state->rx_virtqueue, &handle, &buf, &len, &flag);
        if (more == 0) {
            ZF_LOGF("pico_eth_poll: Invalid virtqueue ring entry");
        }

        if (len > 0) {
            ps_dma_cache_invalidate(&state->io_ops->dma_manager, DECODE_DMA_ADDRESS(buf), len);
            int err = pico_stack_recv(dev, DECODE_DMA_ADDRESS(buf), len);
            if (err <= 0) {
                ZF_LOGE("Failed to rx buffer in poll");
                break;
            }
        }

        virtqueue_get_used_buf(&state->rx_virtqueue, &handle, &len);
        pico_free_buf(DECODE_DMA_ADDRESS(buf));

        loop_score--;
    }
    return loop_score;
}


static void notify_server(UNUSED seL4_Word badge, void *cookie)
{
    state_t *state = cookie;
    if (state->action) {
        state->action = false;
        state->tx_virtqueue.notify();
    }
}

static void irq_from_ethernet(UNUSED seL4_Word badge, void *cookie)
{
    pico_stack_tick();
}

int picotcp_ethernet_async_client_init_late(void *cookie, register_callback_handler_fn_t register_handler)
{
    state_t *data = cookie;
    register_handler(0, "notify_ethernet", notify_server, data);
    return 0;

}

int picotcp_ethernet_async_client_init(ps_io_ops_t *io_ops, const char *tx_virtqueue, const char *rx_virtqueue,
                                       register_callback_handler_fn_t register_handler, get_mac_client_fn_t get_mac, void **cookie)
{
    state_t *data;
    int error = ps_calloc(&io_ops->malloc_ops, 1, sizeof(*data), (void **)&data);
    data->io_ops = io_ops;

    seL4_Word tx_badge;
    seL4_Word rx_badge;
    /* Initialise read virtqueue */
    error = camkes_virtqueue_driver_init_with_recv(&data->tx_virtqueue, camkes_virtqueue_get_id_from_name(tx_virtqueue),
                                                   NULL, &tx_badge);
    if (error) {
        ZF_LOGE("Unable to initialise serial server read virtqueue");
    }

    /* Initialise write virtqueue */
    error = camkes_virtqueue_driver_init_with_recv(&data->rx_virtqueue, camkes_virtqueue_get_id_from_name(rx_virtqueue),
                                                   NULL, &rx_badge);
    if (error) {
        ZF_LOGE("Unable to initialise serial server write virtqueue");
    }

    bool add_to_mapper = false;
    /* preallocate buffers */
    for (int i = 0; i < NUM_RX_BUFS; i++) {
        void *buf = ps_dma_alloc(&io_ops->dma_manager, BUF_SIZE, 64, 1, PS_MEM_NORMAL);
        ZF_LOGF_IF(buf == NULL, "Alloc Failed\n");
        memset(buf, 0, BUF_SIZE);
        virtqueue_ring_object_t handle;

        virtqueue_init_ring_object(&handle);

        /* Save the dataport ID to state data mapping for later.
           pico_free_buf doesn't allow us to provide a cookie so we need to be able
           to get from a dma pointer back to the driver state.
         */
        if (!add_to_mapper) {
            dataport_ptr_t wrapped_ptr = dataport_wrap_ptr(buf);
            mapper = realloc(mapper, mapper_len + 1);
            mapper[mapper_len] = (struct id_state_mapper) {
                .dataport_id = wrapped_ptr.id, .state = data
            };
            mapper_len++;
            add_to_mapper = true;
        }
        if (!virtqueue_add_available_buf(&data->rx_virtqueue, &handle, ENCODE_DMA_ADDRESS(buf), BUF_SIZE, VQ_RW)) {
            ZF_LOGF("Error while enqueuing available buffer, queue full");
        }
    }

    for (int i = 0; i < NUM_TX_BUFS; i++) {
        void *buf = ps_dma_alloc(&io_ops->dma_manager, BUF_SIZE, 64, 1, PS_MEM_NORMAL);
        ZF_LOGF_IF(buf == NULL, "Alloc Failed\n");

        memset(buf, 0, BUF_SIZE);
        data->pending_tx[data->num_tx] = buf;
        data->num_tx++;
    }
    register_handler(tx_badge, "ethernet_event_handler", irq_from_ethernet, data);

    /* Create a driver. This utilises preallocated buffers, backed up by malloc above */
    /* Attach funciton pointers */
    data->pico_dev.send = pico_eth_send;
    data->pico_dev.poll = pico_eth_poll;

    /* Configure the mtu in picotcp */
    uint8_t mac[6] = {0};
    get_mac(&mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    data->pico_dev.mtu = ETHER_MTU;

    if (pico_device_init(&data->pico_dev, "eth0", mac) != 0) {
        ZF_LOGE("Failed to initialize pico device");
        return 1;
    }
    /* Set max frames to reduce unbounded picotcp memory growth. */
    data->pico_dev.q_in->max_frames = 512;
    data->pico_dev.q_out->max_frames = 512;

    printf("Installed eth0 into picotcp\n");
    data->tx_virtqueue.notify();
    *cookie = data;
    return 0;
}
