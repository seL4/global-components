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
#include <assert.h>
#include <stdio.h>
#include <sel4/sel4.h>
#include <sel4/arch/constants.h>
#include <camkes.h>
#include <platsupport/plat/timer.h>
#include <platsupport/irq.h>
#include <utils/util.h>
#include <sel4utils/sel4_zf_logif.h>
#include <simple/simple.h>

#include "../../plat.h"
#include "../../time_server.h"

static ps_irq_t timeout_info = {0};
static ps_irq_t timestamp_info = {0};

void plat_post_init(ltimer_t *ltimer, ps_irq_ops_t *irq_ops)
{
    /* Register the interrupt callbacks */
    timestamp_info = (ps_irq_t) {
        .type = PS_INTERRUPT, .irq = { .number = GPT1_INTERRUPT }
    };
    irq_id_t timestamp_irq_id = ps_irq_register(irq_ops, timestamp_info, time_server_irq_handle, &timestamp_info);
    ZF_LOGF_IF(timestamp_irq_id < 0, "Failed to register IRQ for timestamp");

    timeout_info = (ps_irq_t) {
        .type = PS_INTERRUPT, .irq = { .number = EPIT2_INTERRUPT }
    };
    irq_id_t timeout_irq_id = ps_irq_register(irq_ops, timeout_info, time_server_irq_handle, &timeout_info);
    ZF_LOGF_IF(timeout_irq_id < 0, "Failed to register IRQ for timeout");
}
