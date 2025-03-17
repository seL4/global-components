/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


#include <camkes.h>

/*- set connection_name = configuration[me.parent.name].get('connection_name') -*/

typedef int (*register_callback_handler_fn_t)(seL4_Word badge, const char *,
                                              void (*callback_handler)(seL4_Word, void *),
                                              void *cookie);

int init_lwip_pre(void);

int init_lwip_post(ps_io_ops_t *io_ops, seL4_Word timer_badge, int (*timer_periodic)(int p_tid, uint64_t p_ns), uint64_t (*timer_get_time)(void),
	              const char *ip_addr, const char* multicast_addr_, register_callback_handler_fn_t callback_handler);

int lwip_base_timer_periodic(int p_tid, uint64_t p_ns);

static int _init_lwip_pre(ps_io_ops_t *io_ops)
{
    return init_lwip_pre();
}

CAMKES_ENV_INIT_MODULE_DEFINE(/*? connection_name ?*/_setup_pre, _init_lwip_pre)

static int init_lwip(ps_io_ops_t *io_ops) {

    return init_lwip_post(io_ops, /*? connection_name ?*/_timer_notification_badge(), /*? connection_name ?*/_timer_periodic, /*? connection_name ?*/_timer_time, /*? connection_name ?*/_ip_addr, /*? connection_name ?*/_multicast_addr, single_threaded_component_register_handler);
}

CAMKES_POST_INIT_MODULE_DEFINE(/*? connection_name ?*/_setup_post, init_lwip)
