/*
 * Copyright 2020, Data61, ABN 41 687 119 230.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <picotcp-socket-sync.h>
#include <camkes.h>

/*- set connection_name = configuration[me.parent.name].get('connection_name') -*/

int picotcp_socket_sync_server_init(ps_io_ops_t *io_ops, int num_clients, register_callback_handler_fn_t callback_handler);
int picotcp_socket_sync_server_init_late(register_callback_handler_fn_t callback_handler);

unsigned int /*? connection_name ?*/_recv_num_badges(void);

static int init_server_post(ps_io_ops_t *io_ops) {
    return picotcp_socket_sync_server_init_late(single_threaded_component_register_handler);
}
static int init_server_pre(ps_io_ops_t *io_ops) {
    return picotcp_socket_sync_server_init(io_ops, /*? connection_name ?*/_recv_num_badges(), single_threaded_component_register_handler);
}

CAMKES_PRE_INIT_MODULE_DEFINE(/*? connection_name ?*/_server_setup2, init_server_pre);
CAMKES_POST_INIT_MODULE_DEFINE(/*? connection_name ?*/_server_setup, init_server_post);
