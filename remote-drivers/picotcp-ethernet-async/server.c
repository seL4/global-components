/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include <autoconf.h>
#include <stdbool.h>

#include <camkes/dma.h>
#include <camkes/dataport.h>
#include <camkes/io.h>
#include <camkes/irq.h>
#include <camkes/virtqueue.h>

#include <platsupport/io.h>
#include <platsupport/irq.h>
#include <platsupport/interface_registration.h>
#include <ethdrivers/raw.h>
#include <ethdrivers/intel.h>
#include <sel4utils/sel4_zf_logif.h>
#include <virtqueue.h>
#include <picotcp-ethernet-async.h>

typedef struct data {
    ps_io_ops_t *io_ops;
    virtqueue_device_t tx_virtqueue;
    virtqueue_device_t rx_virtqueue;
    bool action;
    bool blocked_tx;
    bool no_rx_bufs;
    uint8_t hw_mac[6];
    struct eth_driver *eth_driver;
} server_data_t;

#define BUF_SIZE 2048

static void eth_tx_complete(void *iface, void *cookie)
{
    server_data_t *state = iface;

    virtqueue_ring_object_t handle;
    handle.first = (uint32_t)(uintptr_t)cookie;
    handle.cur = (uint32_t)(uintptr_t)cookie;
    if (!virtqueue_add_used_buf(&state->tx_virtqueue, &handle, BUF_SIZE)) {
        ZF_LOGF("eth_tx_complete: Error while enqueuing used buffer, queue full");
    }
    if (state->blocked_tx) {
        state->action = true;
    }
}

static uintptr_t eth_allocate_rx_buf(void *iface, size_t buf_size, void **cookie)
{
    if (buf_size > BUF_SIZE) {
        return 0;
    }
    server_data_t *state = iface;

    virtqueue_ring_object_t handle;

    if (virtqueue_get_available_buf(&state->rx_virtqueue, &handle) == 0) {
        // No buffer available to fill RX ring with.
        state->no_rx_bufs = true;
        return 0;
    }
    state->no_rx_bufs = false;
    void *buf;
    unsigned len;
    vq_flags_t flag;
    int more = virtqueue_gather_available(&state->rx_virtqueue, &handle, &buf, &len, &flag);
    if (more == 0) {
        ZF_LOGF("eth_allocate_rx_buf: Invalid virtqueue ring entry");
    }

    uintptr_t phys = ps_dma_pin(&state->io_ops->dma_manager, DECODE_DMA_ADDRESS(buf), BUF_SIZE);
    *cookie = (void *)(uintptr_t) handle.first;
    return phys;
}

static void eth_rx_complete(void *iface, unsigned int num_bufs, void **cookies, unsigned int *lens)
{
    server_data_t *state = iface;
    if (num_bufs != 1) {
        ZF_LOGE("Dropping packets because num_received didn't match descriptor");
        for (int i = 0; i < num_bufs; i++) {
            virtqueue_ring_object_t handle;
            handle.first = (uintptr_t)cookies[i];
            handle.cur = (uintptr_t)cookies[i];
            if (!virtqueue_add_used_buf(&state->rx_virtqueue, &handle, 0)) {
                ZF_LOGF("eth_rx_complete: Error while enqueuing used buffer, queue full");
            }
        }
        state->action = true;
        return;

    }
    virtqueue_ring_object_t handle;
    handle.first = (uintptr_t)cookies[0];
    handle.cur = (uintptr_t)cookies[0];
    if (!virtqueue_add_used_buf(&state->rx_virtqueue, &handle, lens[0])) {
        ZF_LOGF("eth_rx_complete: Error while enqueuing used buffer, queue full");
    }
    state->action = true;
    return;
}

static struct raw_iface_callbacks ethdriver_callbacks = {
    .tx_complete = eth_tx_complete,
    .rx_complete = eth_rx_complete,
    .allocate_rx_buf = eth_allocate_rx_buf
};



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

static void virt_queue_handle_irq(seL4_Word badge, void *cookie)
{
    server_data_t *state = cookie;
    if (state->no_rx_bufs) {
        state->eth_driver->i_fn.raw_poll(state->eth_driver);
    }
    while (1) {

        virtqueue_ring_object_t handle;

        unsigned next = (state->tx_virtqueue.a_ring_last_seen + 1) & (state->tx_virtqueue.queue_len - 1);

        if (next == state->tx_virtqueue.avail_ring->idx) {
            break;
        }
        handle.first = state->tx_virtqueue.avail_ring->ring[next];
        handle.cur = handle.first;

        void *buf;
        unsigned len;
        vq_flags_t flag;
        int more = virtqueue_gather_available(&state->tx_virtqueue, &handle, &buf, &len, &flag);
        if (more == 0) {
            ZF_LOGF("tx_queue_handle_irq: Invalid virtqueue ring entry");
        }

        uintptr_t phys = ps_dma_pin(&state->io_ops->dma_manager, DECODE_DMA_ADDRESS(buf), BUF_SIZE);
        int err = state->eth_driver->i_fn.raw_tx(state->eth_driver, 1, (uintptr_t *) &phys, (unsigned int *)&len,
                                                 (void *)(uintptr_t)handle.first);
        if (err != ETHIF_TX_ENQUEUED) {
            state->blocked_tx = true;
            break;
        } else {
            state->blocked_tx = false;
            virtqueue_get_available_buf(&state->tx_virtqueue, &handle);
        }
    }

}


static void notify_client(UNUSED seL4_Word badge, void *cookie)
{
    server_data_t *state = cookie;
    if (state->action) {
        if (state->blocked_tx) {
            virt_queue_handle_irq(badge, cookie);
        }
        state->action = false;
        state->tx_virtqueue.notify();
    }
}


static int hardware_interface_searcher(void *cookie, void *interface_instance, char **properties)
{

    server_data_t *state = cookie;
    state->eth_driver = interface_instance;
    return PS_INTERFACE_FOUND_MATCH;
}

int picotcp_ethernet_async_server_init(ps_io_ops_t *io_ops, const char *tx_virtqueue, const char *rx_virtqueue,
                                       register_callback_handler_fn_t register_handler, register_get_mac_server_fn register_get_mac_fn)
{

    server_data_t *data;
    int error = ps_calloc(&io_ops->malloc_ops, 1, sizeof(*data), (void **)&data);
    data->io_ops = io_ops;


    error = ps_interface_find(&io_ops->interface_registration_ops,
                              PS_ETHERNET_INTERFACE, hardware_interface_searcher, data);
    if (error) {
        ZF_LOGF("Unable to find an ethernet device");
    }

    data->eth_driver->cb_cookie = data;
    data->eth_driver->i_cb = ethdriver_callbacks;

    seL4_Word tx_badge;
    seL4_Word rx_badge;

    /* Initialise read virtqueue */
    error = camkes_virtqueue_device_init_with_recv(&data->tx_virtqueue, camkes_virtqueue_get_id_from_name(tx_virtqueue),
                                                   NULL, &tx_badge);
    if (error) {
        ZF_LOGE("Unable to initialise serial server read virtqueue");
    }
    /* Initialise write virtqueue */
    error = camkes_virtqueue_device_init_with_recv(&data->rx_virtqueue, camkes_virtqueue_get_id_from_name(rx_virtqueue),
                                                   NULL, &rx_badge);
    if (error) {
        ZF_LOGE("Unable to initialise serial server write virtqueue");
    }

    error = register_handler(tx_badge, "tx_event", virt_queue_handle_irq, data);
    if (error) {
        ZF_LOGE("Unable to register handler");
    }
    error = register_handler(rx_badge, "rx_event", virt_queue_handle_irq, data);
    if (error) {
        ZF_LOGE("Unable to register handler");
    }

    error = register_handler(0, "notify_client", notify_client, data);
    if (error) {
        ZF_LOGE("Unable to register handler");
    }


    data->eth_driver->i_fn.get_mac(data->eth_driver, data->hw_mac);
    data->eth_driver->i_fn.raw_poll(data->eth_driver);

    register_get_mac_fn(client_get_mac, data);
    return 0;
}
