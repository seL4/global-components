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

#include <assert.h>
#include <string.h>
#include <errno.h>
#include <camkes.h>
#include <camkes/io.h>
#include <utils/util.h>
#include <gpiomuxserver.h>

/*- set connection_name = configuration[me.parent.name].get('connection_name') -*/
mux_sys_t /*? connection_name?*/_server_wrapper;

static int MUXServer_client_init(ps_io_ops_t *io_ops) {

    int error = gpiomuxserver_mux_interface_init(/*? connection_name?*/_feature_enable, /*? connection_name?*/_feature_disable,
                                              &/*? connection_name?*/_server_wrapper);
    if (error) {
        ZF_LOGE("Failed to initialise the GPIOMUXServer interface");
        return error;
    }

    return 0;
}

CAMKES_PRE_INIT_MODULE_DEFINE(mux_client_setup, MUXServer_client_init);
