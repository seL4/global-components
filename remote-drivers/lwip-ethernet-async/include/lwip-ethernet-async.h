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

#include <sel4/sel4.h>
#include <platsupport/io.h>

#define ENCODE_DMA_ADDRESS(buf) ({ \
    dataport_ptr_t wrapped_ptr = dataport_wrap_ptr(buf); \
    ZF_LOGF_IF(wrapped_ptr.id == -1, "Failed to encode DMA address"); \
    uint64_t new_buf = (((uint64_t)wrapped_ptr.id << 32) | ((uint64_t)wrapped_ptr.offset)); \
    new_buf; })

#define DECODE_DMA_ADDRESS(buf) ({\
    dataport_ptr_t wrapped_ptr = {.id = ((uint64_t)buf >> 32), .offset = (uint64_t)buf & MASK(32)}; \
    void *ptr = dataport_unwrap_ptr(wrapped_ptr); \
    ZF_LOGF_IF(ptr == NULL, "Failed to decode DMA address"); \
    ptr; })

typedef void(*get_mac_server_fn_t)(uint8_t *b1, uint8_t *b2, uint8_t *b3, uint8_t *b4, uint8_t *b5, uint8_t *b6,
                                   void *cookie);
typedef void(*get_mac_client_fn_t)(uint8_t *b1, uint8_t *b2, uint8_t *b3, uint8_t *b4, uint8_t *b5, uint8_t *b6);

typedef int (*register_callback_handler_fn_t)(seL4_Word badge, const char *, void (*callback_handler)(seL4_Word,
                                                                                                      void *),
                                              void *cookie);
typedef void (*register_get_mac_server_fn)(get_mac_server_fn_t get_mac, void *cookie);

typedef int (*register_cb_fn)(void (*cb)(void*), void *arg);

typedef void (*notify_fn)(void);

int lwip_ethernet_async_client_init(ps_io_ops_t *io_ops, get_mac_client_fn_t get_mac, 
                void *rx_avail, void *rx_used, void *tx_avail, void *tx_used,  
                register_cb_fn reg_rx_cb, notify_fn tx_notify);


int lwip_ethernet_async_server_init(ps_io_ops_t *io_ops, register_get_mac_server_fn register_get_mac_fn,
                void *rx_avail, void *rx_used, void *tx_avail, void *tx_used, 
                register_cb_fn reg_tx_cb, notify_fn rx_notify);