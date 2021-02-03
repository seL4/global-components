/*
 * Copyright 2020, Data61, ABN 41 687 119 230.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <platsupport/io.h>
#include <vka/vka.h>
#include <simple/simple.h>
#include <simple/simple_helpers.h>
#include <allocman/vka.h>
#include <allocman/bootstrap.h>
#include <sel4utils/vspace.h>
#include <camkes.h>


static allocman_t *allocman;
static char allocator_mempool[0x800000];
static simple_t camkes_simple;
static vka_t vka;
static vspace_t vspace;
static sel4utils_alloc_data_t vspace_data;

void camkes_make_simple(simple_t *simple);



static USED SECTION("_dynamic_init") struct {} dummy_dynamic_init_module;

extern camkes_dynamic_module_init_fn_t __start__dynamic_init[];
extern camkes_dynamic_module_init_fn_t __stop__dynamic_init[];

static int init_system(ps_io_ops_t *io_ops)
{
    int error;

    /* Camkes adds nothing to our address space, so this array is empty */
    void *existing_frames[] = {
        NULL
    };
    camkes_make_simple(&camkes_simple);

    /* Initialize allocator */
    allocman = bootstrap_use_current_1level(
                   simple_get_cnode(&camkes_simple),
                   simple_get_cnode_size_bits(&camkes_simple),
                   simple_last_valid_cap(&camkes_simple) + 1,
                   BIT(simple_get_cnode_size_bits(&camkes_simple)),
                   sizeof(allocator_mempool), allocator_mempool
               );
    assert(allocman);
    error = allocman_add_simple_untypeds(allocman, &camkes_simple);
    allocman_make_vka(&vka, allocman);

    /* Initialize the vspace */
    error = sel4utils_bootstrap_vspace(&vspace, &vspace_data,
                                       simple_get_init_cap(&camkes_simple, seL4_CapInitThreadPD), &vka, NULL, NULL, existing_frames);
    assert(!error);


    for (camkes_dynamic_module_init_fn_t * init_fun = __start__dynamic_init; init_fun < __stop__dynamic_init; init_fun++) {
        error = (*init_fun)(io_ops, &vka, &camkes_simple, &vspace);
        if (error) {
            ZF_LOGE("Failed to initialize %p", init_fun);
        }

    }

    return 0;
}


CAMKES_ENV_INIT_MODULE_DEFINE(dynamic_untyped_allocators_init, init_system);
