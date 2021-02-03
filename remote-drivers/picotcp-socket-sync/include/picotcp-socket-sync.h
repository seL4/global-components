/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


#include <stdint.h>
#include <sel4/sel4.h>
#include <platsupport/io.h>


typedef int (*register_callback_handler_fn_t)(seL4_Word badge, const char *, void (*callback_handler)(seL4_Word,
                                                                                                      void *),
                                              void *cookie);
