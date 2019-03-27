/*
 * Copyright 2018, Data61
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

void ttc0_irq_t1_handle() {
    ps_irq_t irq = { .type = PS_INTERRUPT, .irq = { .number = TTC0_TIMER1_IRQ }};
    time_server_irq_handle(ttc0_irq_t1_acknowledge, &irq);
}

void ttc0_irq_t2_handle() {
    /* We don't actually use this timer */
    time_server_irq_handle(ttc0_irq_t2_acknowledge, NULL);
}

void ttc0_irq_t3_handle() {
    /* We don't actually use this timer */
    time_server_irq_handle(ttc0_irq_t3_acknowledge, NULL);
}

void ttc1_irq_t1_handle() {
    ps_irq_t irq = { .type = PS_INTERRUPT, .irq = { .number = TTC1_TIMER1_IRQ }};
    time_server_irq_handle(ttc1_irq_t1_acknowledge, &irq);
}
void ttc1_irq_t2_handle() {
    /* We don't actually use this timer */
    time_server_irq_handle(ttc1_irq_t2_acknowledge, NULL);
}

void ttc1_irq_t3_handle() {
    /* We don't actually use this timer */
    time_server_irq_handle(ttc1_irq_t3_acknowledge, NULL);
}

void plat_post_init(ltimer_t *ltimer) {
    int error;

    error = ttc0_irq_t1_acknowledge();
    ZF_LOGF_IF(error, "Failed to ack ttc0 irq t1 irq");

    error = ttc0_irq_t2_acknowledge();
    ZF_LOGF_IF(error, "Failed to ack ttc0 irq t2 irq");

    error = ttc0_irq_t3_acknowledge();
    ZF_LOGF_IF(error, "Failed to ack ttc0 irq t3 irq");

    error = ttc1_irq_t1_acknowledge();
    ZF_LOGF_IF(error, "Failed to ack ttc1 irq t1 irq");

    error = ttc1_irq_t2_acknowledge();
    ZF_LOGF_IF(error, "Failed to ack ttc1 irq t2 irq");

    error = ttc1_irq_t3_acknowledge();
    ZF_LOGF_IF(error, "Failed to ack ttc1 irq t3 irq");

}
