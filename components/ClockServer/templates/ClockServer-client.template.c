/*
 * Copyright 2020, Data61, ABN 41 687 119 230.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <assert.h>
#include <string.h>
#include <errno.h>
#include <camkes.h>
#include <camkes/io.h>
#include <utils/util.h>
#include <clockserver_client.h>

/*- set connection_name = configuration[me.parent.name].get('connection_name') -*/
static clock_sys_t /*? connection_name?*/_server_wrapper;

static int ClockServer_client_init(ps_io_ops_t *io_ops) {
    int error = clockserver_interface_init(io_ops, /*? connection_name ?*/_init_clock, /*? connection_name ?*/_set_gate_mode,
                                       /*? connection_name ?*/_get_freq, /*? connection_name ?*/_set_freq, /*? connection_name ?*/_register_child,
                                       &/*? connection_name ?*/_server_wrapper);
    if (error) {
        ZF_LOGE("Failed to initialise the ClockServer interface");
        return error;
    }
    /* Register these interfaces against the interface registration service. */
    error = ps_interface_register(&io_ops->interface_registration_ops, PS_CLOCK_INTERFACE,
                                  &clock_server_wrapper, NULL);
    if (error) {
        ZF_LOGE("Failed to register the ResetServer interface");
        return error;
    }

    return 0;
}

CAMKES_PRE_INIT_MODULE_DEFINE(clock_client_setup, ClockServer_client_init);
