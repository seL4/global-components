/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <picotcp-ethernet-async.h>
#include <camkes.h>

/*- set connection_name = configuration[me.parent.name].get('connection_name') -*/

static void *instance_cookie;
static int init_client_pre(ps_io_ops_t *io_ops) {

    return picotcp_ethernet_async_client_init(io_ops, "/*? connection_name ?*/_tx", "/*? connection_name ?*/_rx", single_threaded_component_register_handler,
             /*? connection_name ?*/_control_mac, &instance_cookie);
}

static int init_client_post(ps_io_ops_t *io_ops) {

    return picotcp_ethernet_async_client_init_late(instance_cookie, single_threaded_component_register_handler);
}
CAMKES_PRE_INIT_MODULE_DEFINE(/*? connection_name ?*/_client_setup, init_client_pre);
CAMKES_POST_INIT_MODULE_DEFINE(/*? connection_name ?*/_client_setup2, init_client_post);
