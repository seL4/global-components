/*
 * Copyright 2019, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */

#include <autoconf.h>
#include <sel4/sel4.h>
#include <camkes.h>
#include <utils/util.h>
#include <sel4utils/sel4_zf_logif.h>

#include "../../plat.h"
#include "../../serial.h"

int plat_serial_interrupt_acknowledge() {
    return serial_irq_acknowledge();
}

void serial_irq_handle()
{
    serial_server_irq_handle();
}
