/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <camkes.h>

static void __attribute__((constructor)) register_serial(void) {
    set_putchar(/*? me.interface ?*/_putchar_putchar);
}

