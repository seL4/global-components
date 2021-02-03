/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <assert.h>
#include <string.h>
#include <errno.h>
#include <camkes.h>
#include <camkes/io.h>
#include <utils/util.h>
#include <resetserver_client.h>

/*- set connection_name = configuration[me.parent.name].get('connection_name') -*/
static reset_sys_t /*? connection_name?*/_server_wrapper;

static int ResetServer_client_init(ps_io_ops_t *io_ops) {
    int error = resetserver_interface_init(/*? connection_name?*/_assert_reset, /*? connection_name?*/_deassert_reset,
                                       &/*? connection_name?*/_server_wrapper);
    if (error) {
        ZF_LOGE("Failed to initialise the ResetServer interface");
        return error;
    }
    /* Register these interfaces against the interface registration service. */
    error = ps_interface_register(&io_ops->interface_registration_ops, PS_RESET_INTERFACE,
                                  &reset_server_wrapper, NULL);
    if (error) {
        ZF_LOGE("Failed to register the ResetServer interface");
        return error;
    }

    return 0;
}

CAMKES_PRE_INIT_MODULE_DEFINE(reset_client_setup, ResetServer_client_init);
