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


#include <stdint.h>
#include <sel4/sel4.h>
#include <platsupport/io.h>


typedef int (*register_callback_handler_fn_t)(seL4_Word badge, const char*, void (*callback_handler)(seL4_Word, void *),
                                              void *cookie);
