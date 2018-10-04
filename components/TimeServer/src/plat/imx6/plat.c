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

#define GPT_ADDR (unsigned int)gpt_mem
#define EPIT2_ADDR (unsigned int)epit2_mem

void epit2_irq_handle() {
    time_server_irq_handle(epit2_irq_acknowledge);
}

void gpt_irq_handle() {
    time_server_irq_handle(gpt_irq_acknowledge);
}

void plat_post_init(ltimer_t *ltimer, ps_io_ops_t ops) {
    int error;

    static_timer_params_t timer_params;
    timer_params.timestamp_vaddr = (void *)GPT_ADDR;
    timer_params.timeout_vaddr = (void *)EPIT2_ADDR;

    error = ltimer_static_init(ltimer, ops, (void *)&timer_params);
    ZF_LOGF_IF(error, "Failed to get timer");

    error = gpt_irq_acknowledge();
    ZF_LOGF_IF(error, "Failed to ack gpt irq");

    error = epit2_irq_acknowledge();
    ZF_LOGF_IF(error, "Failed to ack epit2 irq");
}
