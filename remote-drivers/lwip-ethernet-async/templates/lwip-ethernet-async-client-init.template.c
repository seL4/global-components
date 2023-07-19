/*
 * Copyright 2022, UNSW (ABN 57 195 873 179)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <lwip-ethernet-async.h>
#include <camkes.h>

/*- set connection_name = configuration[me.parent.name].get('connection_name') -*/

static int init_client_pre(ps_io_ops_t *io_ops) {

    return lwip_ethernet_async_client_init(io_ops, /*? connection_name ?*/_control_mac, 
                    rx_avail, rx_used, tx_avail, tx_used, 
                    rx_done_reg_callback, tx_ready_emit);
}

CAMKES_PRE_INIT_MODULE_DEFINE(/*? connection_name ?*/_client_setup_pre, init_client_pre);

static int init_client_post(ps_io_ops_t *io_ops) {  
    return 0;
}
CAMKES_POST_INIT_MODULE_DEFINE(/*? connection_name ?*/_client_setup_late, init_client_post);
