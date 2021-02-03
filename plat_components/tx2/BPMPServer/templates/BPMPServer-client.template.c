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
#include <bpmpserver_client.h>

/*- set connection_name = configuration[me.parent.name].get('connection_name') -*/
static struct tx2_bpmp /*? connection_name?*/_server_wrapper;
extern void */*? connection_name ?*/_buf;
size_t /*? connection_name ?*/_get_size(void);

static int BPMPServer_client_init(ps_io_ops_t *io_ops) {
    int error = bpmpserver_interface_init(/*? connection_name ?*/_buf, /*? connection_name ?*/_get_size(), /*? connection_name ?*/_call, &/*? connection_name?*/_server_wrapper);
    if (error) {
        ZF_LOGE("Failed to initialise the BPMP server client interface");
        return -1;
    }

    error = ps_interface_register(&io_ops->interface_registration_ops, TX2_BPMP_INTERFACE,
                                  &/*? connection_name?*/_server_wrapper, NULL);
    if (error) {
        ZF_LOGE("Failed to register the BPMP interface");
        return -1;
    }

    return 0;
}

CAMKES_PRE_INIT_MODULE_DEFINE(bpmp_client_setup, BPMPServer_client_init);
