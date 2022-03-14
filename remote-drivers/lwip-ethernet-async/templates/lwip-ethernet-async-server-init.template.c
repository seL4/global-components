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

#include <lwip-ethernet-async.h>
#include <camkes.h>
#include <sel4/syscalls.h>

/*- set connection_name = configuration[me.parent.name].get('connection_name') -*/

void default_get_mac(uint8_t *b1, uint8_t *b2, uint8_t *b3, uint8_t *b4, uint8_t *b5, uint8_t *b6, void *cookie) {
    ZF_LOGE("/*? configuration[me.parent.name].get('connection_name') ?*/_control_mac has not been registered on server side");
    return;
}

static get_mac_server_fn_t get_mac_dispatch = default_get_mac;
static void *get_mac_dispatch_cookie;

static void register_get_mac_fn(get_mac_server_fn_t get_mac, void *cookie)
{
    get_mac_dispatch = get_mac;
    get_mac_dispatch_cookie = cookie;
}

void /*? configuration[me.parent.name].get('connection_name') ?*/_control_mac(uint8_t *b1, uint8_t *b2, uint8_t *b3, uint8_t *b4, uint8_t *b5, uint8_t *b6) {
    if (get_mac_dispatch) {
        get_mac_dispatch(b1, b2, b3, b4, b5, b6, get_mac_dispatch_cookie);
    } else {
        ZF_LOGE("/*? configuration[me.parent.name].get('connection_name') ?*/_control_mac has been removed on server side");
    }
}

static int init_server(ps_io_ops_t *io_ops) {
    return lwip_ethernet_async_server_init(io_ops, register_get_mac_fn, 
                rx_avail, rx_used, tx_avail, tx_used, tx_ready_reg_callback, rx_done_emit);
}

CAMKES_POST_INIT_MODULE_DEFINE(/*? connection_name ?*/_server_setup, init_server);
