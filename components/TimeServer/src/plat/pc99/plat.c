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
#include <sel4utils/arch/tsc.h>

#include "../../time_server.h"
#include "../../plat.h"

static uint64_t tsc_frequency = 0;

uint64_t the_timer_tsc_frequency() {
    return tsc_frequency;
}

static int pit_port_in(void *cookie, uint32_t port, int io_size, uint32_t *result) {
    if (io_size != 1) {
        return -1;
    }
    switch(port) {
    case 0x43:
        *result = pit_command_in8(port);
        return 0;
    case 0x40:
        *result = pit_channel0_in8(port);
        return 0;
    default:
        return -1;
    }
}

static int pit_port_out(void *cookie, uint32_t port, int io_size, uint32_t val) {
    if (io_size != 1) {
        return -1;
    }
    switch(port) {
    case 0x43:
        pit_command_out8(port, val);
        return 0;
    case 0x40:
        pit_channel0_out8(port, val);
        return 0;
    default:
        return -1;
    }
}

void irq_handle(void) {
    time_server_irq_handle(irq_acknowledge);
}

// We declare this with a weak attribute here as we would like this component to work
// regardless of whether the assembly declared this to have a simple template or not.
// Having this as weak allows us to test for this at run time / link time
void camkes_make_simple(simple_t *simple) __attribute__((weak));

void plat_post_init(ltimer_t *ltimer, ps_io_ops_t ops) {
    ops.io_port_ops.io_port_in_fn = pit_port_in;
    ops.io_port_ops.io_port_out_fn = pit_port_out;

    int error = ltimer_pit_init(ltimer, ops);
    ZF_LOGF_IF(error, "Failed to get timer");

    error = irq_acknowledge();
    ZF_LOGF_IF(error, "Failed to ack irq");

    // Attempt to detect the presence of a simple interface and try and get the
    // tsc frequency from it
    tsc_frequency = 0;
    if (camkes_make_simple) {
        simple_t simple;
        camkes_make_simple(&simple);
        tsc_frequency = x86_get_tsc_freq_from_simple(&simple);
    }

    if (tsc_frequency == 0) {
        // failed to detect from bootinfo for whatever reason, rely on the pit calibration
        tsc_frequency = ltimer_pit_get_tsc_freq(ltimer);
    }
}
