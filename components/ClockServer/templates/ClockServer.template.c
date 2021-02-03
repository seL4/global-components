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

int ClockServer_init(ps_io_ops_t *io_ops);

CAMKES_POST_INIT_MODULE_DEFINE(Clock_server_setup, ClockServer_init);
