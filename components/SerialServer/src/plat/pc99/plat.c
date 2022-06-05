/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <autoconf.h>
#include <sel4/sel4.h>
#include <camkes.h>
#include <utils/util.h>
#include <platsupport/irq.h>
#include <sel4utils/sel4_zf_logif.h>

#include "../../plat.h"
#include "../../serial.h"

void plat_post_init(ps_irq_ops_t *irq_ops)
{
    ps_irq_t irq_info = { .type = PS_IOAPIC, .ioapic = { .ioapic = 0, .pin = DEFAULT_SERIAL_INTERRUPT, .level = 0, .polarity = 0, .vector = DEFAULT_SERIAL_INTERRUPT }};
    irq_id_t serial_irq_id = ps_irq_register(irq_ops, irq_info, serial_server_irq_handle, NULL);
    ZF_LOGF_IFERR(serial_irq_id < 0, "Failed to register irq for serial");
}
