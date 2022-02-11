/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 * Copyright 2021, Technology Innovation Institute
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#pragma once

#define HARDWARE_TIMER_INTERFACES                                        \
    emits Dummy dummy_source;                                            \
    consumes Dummy system_timer;
#define HARDWARE_TIMER_ATTRIBUTES
#define HARDWARE_TIMER_COMPOSITION                                       \
        connection seL4DTBHardware system_timer_conn(from dummy_source, to system_timer);
#define HARDWARE_TIMER_CONFIG                                            \
        system_timer.dtb = dtb({"path" : "/soc/timer@7e003000"});        \
        system_timer.generate_interrupts = 1;
#define HARDWARE_TIMER_PLAT_INTERFACES
