/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


#include <stdint.h>
#include <sel4/sel4.h>
#include <platsupport/io.h>

#define ENCODE_DMA_ADDRESS(buf) ({ \
    dataport_ptr_t wrapped_ptr = dataport_wrap_ptr(buf); \
    void *new_buf = (void *)(((uintptr_t)wrapped_ptr.id << 32) | ((uintptr_t)wrapped_ptr.offset)); \
    new_buf; })

#define DECODE_DMA_ADDRESS(buf) ({\
        dataport_ptr_t wrapped_ptr = {.id = ((uintptr_t)buf >> 32), .offset = (uintptr_t)buf & MASK(32)}; \
        void *ptr = dataport_unwrap_ptr(wrapped_ptr); \
        ptr; })


typedef void(*get_mac_server_fn_t)(uint8_t *b1, uint8_t *b2, uint8_t *b3, uint8_t *b4, uint8_t *b5, uint8_t *b6,
                                   void *cookie);
typedef void(*get_mac_client_fn_t)(uint8_t *b1, uint8_t *b2, uint8_t *b3, uint8_t *b4, uint8_t *b5, uint8_t *b6);


typedef int (*register_callback_handler_fn_t)(seL4_Word badge, const char *,
                                              void (*callback_handler)(seL4_Word, void *), void *cookie);
typedef void (*register_get_mac_server_fn)(get_mac_server_fn_t get_mac, void *cookie);



int picotcp_ethernet_async_client_init(ps_io_ops_t *io_ops, const char *tx_virtqueue, const char *rx_virtqueue,
                                       register_callback_handler_fn_t register_handler, get_mac_client_fn_t get_mac, void **cookie);
int picotcp_ethernet_async_client_init_late(void *cookie, register_callback_handler_fn_t register_handler);


int picotcp_ethernet_async_server_init(ps_io_ops_t *io_ops, const char *tx_virtqueue, const char *rx_virtqueue,
                                       register_callback_handler_fn_t register_handler, register_get_mac_server_fn register_get_mac_fn);
