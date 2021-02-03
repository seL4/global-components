/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sel4/sel4.h>


typedef int (*camkes_module_init_fn_t)(ps_io_ops_t *io_ops);

#define CAMKES_ENV_INIT_MODULE_DEFINE(name, init_func)                   \
    static_assert(init_func != NULL, "Supplied init_func is NULL!");            \
    USED SECTION("_env_init") camkes_module_init_fn_t name = init_func;

#define CAMKES_PRE_INIT_MODULE_DEFINE(name, init_func)                   \
    static_assert(init_func != NULL, "Supplied init_func is NULL!");            \
    USED SECTION("_pre_init") camkes_module_init_fn_t name = init_func;

#define CAMKES_POST_INIT_MODULE_DEFINE(name, init_func)                   \
    static_assert(init_func != NULL, "Supplied init_func is NULL!");            \
    USED SECTION("_post_init") camkes_module_init_fn_t name = init_func;


/* Function for registering notification event handlers under a certain badge value */
int single_threaded_component_register_handler(seL4_Word badge, const char* name, void (*callback_handler)(seL4_Word, void *), void * cookie);

