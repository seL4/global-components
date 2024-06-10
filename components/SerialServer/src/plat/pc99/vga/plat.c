/*
 * Copyright 2024, Dornerworks, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <autoconf.h>
#include <sel4/sel4.h>
#include <camkes.h>
#include <utils/util.h>
#include <platsupport/irq.h>
#include <sel4utils/sel4_zf_logif.h>

#include "../../../plat.h"
#include "../../../serial.h"

void plat_post_init(ps_irq_ops_t *irq_ops)
{
    ZF_LOGI("EGA Display registers no IRQ");
    return;
}
