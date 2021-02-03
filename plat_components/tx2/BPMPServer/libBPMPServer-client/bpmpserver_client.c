/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <errno.h>
#include <string.h>

#include <utils/util.h>
#include <bpmpserver_client.h>

typedef struct bpmpserver_interface_data {
    void *bpmpserver_shared_mem;
    int (*bpmpserver_call)(int mrq, size_t tx_size, size_t *bytes_rxd);
    size_t shared_mem_size;
} bpmpserver_interface_data_t;

static bpmpserver_interface_data_t bpmpserver_private_data;

static int bpmpserver_call_wrapper(void *data, int mrq, void *tx_msg, size_t tx_size,
                                   void *rx_msg, size_t rx_size)
{
    if (!tx_msg) {
        ZF_LOGE("tx_msg is NULL!");
        return -EINVAL;
    }

    bpmpserver_interface_data_t *bpmpserver_data = data;

    if (tx_size >= bpmpserver_data->shared_mem_size) {
        ZF_LOGE("Supplied buffer is too large!");
        return -EINVAL;
    }

    /* Clean up the shared memory buffer */
    memset(bpmpserver_data->bpmpserver_shared_mem, 0, bpmpserver_data->shared_mem_size);

    /* Copy the tx_msg contents into the shared memory buffer, the buffers
     * shouldn't overlap so memcpy should be safe */
    memcpy(bpmpserver_data->bpmpserver_shared_mem, tx_msg, tx_size);

    size_t bytes_received = 0;
    int ret = bpmpserver_data->bpmpserver_call(mrq, tx_size, &bytes_received);

    if (ret >= 0 && rx_msg) {
        /* Copy the contents of the shared memory region (response from BPMP)
         * into the rx buffer */
        if (bytes_received > rx_size) {
            ZF_LOGW("Response received is larger than the buffer supplied");
        }
        memcpy(rx_msg, bpmpserver_data->bpmpserver_shared_mem, bytes_received);
        /* No error, return the amount of bytes received */
        return bytes_received;
    }

    /* Encountered an error so we return the error code */
    return ret;
}

int bpmpserver_interface_init(void *bpmpserver_shared_mem, size_t shared_mem_size,
                              int (*bpmpserver_call)(int mrq, size_t tx_size, size_t *bytes_rxd),
                              struct tx2_bpmp *bpmp)
{
    if (!bpmpserver_shared_mem) {
        ZF_LOGE("bpmpserver_shared_mem is NULL");
        return -EINVAL;
    }

    if (!bpmpserver_call) {
        ZF_LOGE("bpmpserver_call is NULL");
        return -EINVAL;
    }

    if (!bpmp) {
        ZF_LOGE("bpmp is NULL");
        return -EINVAL;
    }

    /* Setup the private data */
    bpmpserver_private_data.bpmpserver_shared_mem = bpmpserver_shared_mem;
    bpmpserver_private_data.bpmpserver_call = bpmpserver_call;
    bpmpserver_private_data.shared_mem_size = shared_mem_size;

    /* Setup the function pointer */
    bpmp->call = bpmpserver_call_wrapper;
    bpmp->data = &bpmpserver_private_data;

    return 0;
}
