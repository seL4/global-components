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

#include <platsupport/io.h>
#include <vka/vka.h>
#include <simple/simple.h>
#include <simple/simple_helpers.h>
#include <allocman/vka.h>
#include <allocman/bootstrap.h>
#include <sel4utils/vspace.h>
#include <camkes.h>


/* 
   TODO: This module constructs a vspace and vka backed by an allocman instance
   that is supplied by untypeds.  It overwrites the malloc static pool with
   a dynamic vspace brk implementation.  There are a number of configuration
   options:
  - input collection of untypeds that are fed to the allocater provided by the
    simple camkes component environment.
  - BRK_VIRTUAL_SIZE: Size of malloc brk section.
  - allocator_mempool: Size of the static mempool provided to the allocator on
    init.
  - virtual mempool size: Whether to create a virtual mempool that gets allocated
    out of the supplied untyped.  Currently there is no virtual mempool.

  These configurations should be exported to the camkes ADL level.

  Some relationships to other camkes environment:
    - the camkes component heap size should be 0 if this overrides malloc.
    - Or optionally this component adds in the heap_size to it's own
      allocation.

  Total allocation:
   Static allocation
   virtual = total - static

 */
#define BRK_VIRTUAL_SIZE 400000000

reservation_t muslc_brk_reservation;
void *muslc_brk_reservation_start;
vspace_t  *muslc_this_vspace;
static sel4utils_res_t muslc_brk_reservation_memory;
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

    sel4utils_reserve_range_no_alloc(&vspace, &muslc_brk_reservation_memory, BRK_VIRTUAL_SIZE, seL4_AllRights, 1,
                                     &muslc_brk_reservation_start);
    muslc_this_vspace = &vspace;
    muslc_brk_reservation = (reservation_t) {
        .res = &muslc_brk_reservation_memory
    };

    for (camkes_dynamic_module_init_fn_t * init_fun = __start__dynamic_init; init_fun < __stop__dynamic_init; init_fun++) {
        error = (*init_fun)(io_ops, &vka, &camkes_simple, &vspace);
        if (error) {
            ZF_LOGE("Failed to initialize %p", init_fun);
        }

    }

    return 0;
}


CAMKES_ENV_INIT_MODULE_DEFINE(dynamic_untyped_allocators_init, init_system);
