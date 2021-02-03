/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <platsupport/io.h>
#include <vka/vka.h>
#include <simple/simple.h>
#include <sel4utils/vspace.h>

#define CAMKES_DYNAMIC_INIT_MODULE_DEFINE(init_func)                   \
    static_assert(init_func != NULL, "Supplied init_func is NULL!");            \
    USED SECTION("_dynamic_init") camkes_dynamic_module_init_fn_t init_func##_ref = init_func;

typedef int (*camkes_dynamic_module_init_fn_t)(ps_io_ops_t *io_ops, vka_t *vka, simple_t *camkes_simple, vspace_t *vspace);

