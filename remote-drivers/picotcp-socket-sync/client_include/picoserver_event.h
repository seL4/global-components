/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#pragma once

#include <camkes/dataport.h>

typedef struct picoserver_event {
    int socket_fd;
    uint16_t events;
    int num_events_left;
} picoserver_event_t;
#define ENCODE_DMA_ADDRESS(buf) ({ \
    dataport_ptr_t wrapped_ptr = dataport_wrap_ptr(buf); \
    void *new_buf = (void *)(((uintptr_t)wrapped_ptr.id << 32) | ((uintptr_t)wrapped_ptr.offset)); \
    new_buf; })

#define DECODE_DMA_ADDRESS(buf) ({\
        dataport_ptr_t wrapped_ptr = {.id = ((uintptr_t)buf >> 32), .offset = (uintptr_t)buf & MASK(32)}; \
        void *ptr = dataport_unwrap_ptr(wrapped_ptr); \
        ptr; })

typedef struct tx_msg tx_msg_t;
struct tx_msg {
    int socket_fd;
    size_t total_len;
    size_t done_len;
    uint32_t src_addr;
    uint16_t remote_port;
    tx_msg_t *next;
    void *cookie_save;
    void *client_cookie;
    char buf[];
};
