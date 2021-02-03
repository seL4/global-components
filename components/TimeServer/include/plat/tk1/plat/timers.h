/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#pragma once

#define HARDWARE_TIMER_INTERFACES                                                   \
    consumes Dummy nv_timer;                                                        \
    emits Dummy dummy_source;
#define HARDWARE_TIMER_ATTRIBUTES
#define HARDWARE_TIMER_COMPOSITION                                                  \
        connection seL4DTBHardware nv_timer_conn(from dummy_source, to nv_timer);
#define HARDWARE_TIMER_CONFIG                                                       \
        nv_timer.dtb = dtb({"path" : "/timer@60005000"});                           \
        nv_timer.generate_interrupts = 1;
#define HARDWARE_TIMER_PLAT_INTERFACES
