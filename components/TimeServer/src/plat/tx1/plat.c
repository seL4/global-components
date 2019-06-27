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

#include <camkes.h>
#include <platsupport/plat/timer.h>
#include <platsupport/irq.h>
#include <sel4utils/sel4_zf_logif.h>

#include "../../plat.h"
#include "../../time_server.h"

static ps_irq_t timer_info = {0};

void plat_post_init(ltimer_t *ltimer, ps_irq_ops_t *irq_ops)
{
    /* Register the interrupt callbacks,
     * TX1 uses only one timer for both timeout and timestamp */
    timer_info = (ps_irq_t) {
        .type = PS_INTERRUPT, .irq = { .number = INT_NV_TMR1 }
    };
    irq_id_t timer_irq_id = ps_irq_register(irq_ops, timer_info, time_server_irq_handle, &timer_info);
    ZF_LOGF_IF(timer_irq_id < 0, "Failed to register IRQ for timer");
}
