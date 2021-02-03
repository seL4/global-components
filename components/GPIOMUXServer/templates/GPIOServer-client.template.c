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
#include <gpiomuxserver.h>

/*- set connection_name = configuration[me.parent.name].get('connection_name') -*/
static gpio_sys_t /*? connection_name?*/_server_wrapper;

static int GPIOServer_client_init(ps_io_ops_t *io_ops) {

    int error = gpiomuxserver_gpio_interface_init(/*? connection_name?*/_init_pin, /*? connection_name?*/_set_level, /*? connection_name?*/_read_level,
                                              &/*? connection_name?*/_server_wrapper);
    if (error) {
        ZF_LOGE("Failed to initialise the GPIOMUXServer interface");
        return error;
    }
    /* Register these interfaces against the interface registration service. */
    error = ps_interface_register(&io_ops->interface_registration_ops, PS_GPIO_INTERFACE,
                                  &gpio_server_wrapper, NULL);
    if (error) {
        ZF_LOGE("Failed to register the GPIOMUXServer interface");
        return error;
    }
    return 0;
}

CAMKES_PRE_INIT_MODULE_DEFINE(gpio_client_setup, GPIOServer_client_init);
