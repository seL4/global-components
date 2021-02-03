/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#pragma once

#define HARDWARE_TIMER_INTERFACES                                                   \
    consumes Dummy epit2;                                                           \
    consumes Dummy gpt;                                                             \
    emits Dummy dummy_source;
#define HARDWARE_TIMER_ATTRIBUTES
#define HARDWARE_TIMER_COMPOSITION                                                  \
        connection seL4DTBHardware epit2_conn(from dummy_source, to epit2);         \
        connection seL4DTBHardware gpt_conn(from dummy_source, to gpt);
#define HARDWARE_TIMER_CONFIG                                                                       \
        epit2.dtb = dtb({"path" : "/soc/aips-bus@2000000/epit@20d4000"});                           \
        epit2.generate_interrupts = 1;                                                              \
        gpt.dtb = dtb({"path" : "/soc/aips-bus@2000000/gpt@2098000"});                              \
        gpt.generate_interrupts = 1;
#define HARDWARE_TIMER_PLAT_INTERFACES
