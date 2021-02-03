/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <platsupport/io.h>
#include <utils/attribute.h>

int mux_component_init(ps_io_ops_t *io_ops);

WEAK int mux_component_plat_init(ps_io_ops_t *io_ops);
