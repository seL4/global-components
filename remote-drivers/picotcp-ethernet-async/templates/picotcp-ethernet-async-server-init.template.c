/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <picotcp-ethernet-async.h>
#include <camkes.h>

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
    /* initialize seL4 allocators and give us a half sane environment */
    return picotcp_ethernet_async_server_init(io_ops, "/*? connection_name ?*/_tx", "/*? connection_name ?*/_rx", single_threaded_component_register_handler, register_get_mac_fn);
}

CAMKES_POST_INIT_MODULE_DEFINE(/*? connection_name ?*/_server_setup, init_server);
