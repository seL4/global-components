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
#include <utils/util.h>
#include <sel4utils/sel4_zf_logif.h>
#include <simple/simple.h>

#include "../../plat.h"
#include "../../time_server.h"

#define TTC0_ADDR (unsigned int)ttc0_mem
#define TTC1_ADDR (unsigned int)ttc1_mem

void ttc0_irq_t1_handle() {
    time_server_irq_handle(ttc0_irq_t1_acknowledge);
}

void ttc0_irq_t2_handle() {
    time_server_irq_handle(ttc0_irq_t2_acknowledge);
}

void ttc0_irq_t3_handle() {
    time_server_irq_handle(ttc0_irq_t3_acknowledge);
}

void ttc1_irq_t1_handle() {
    time_server_irq_handle(ttc1_irq_t1_acknowledge);
}
void ttc1_irq_t2_handle() {
    time_server_irq_handle(ttc1_irq_t2_acknowledge);
}

void ttc1_irq_t3_handle() {
    time_server_irq_handle(ttc1_irq_t3_acknowledge);
}

void plat_post_init(ltimer_t *ltimer, ps_io_ops_t ops) {
    int error;

    static_timer_params_t timer_params;
    timer_params.timeout_vaddr = (void *)TTC0_ADDR;
    timer_params.timestamp_vaddr = (void *)TTC1_ADDR;

    error = ltimer_static_init(ltimer, ops, (void *)&timer_params);
    ZF_LOGF_IF(error, "Failed to get timer");

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
