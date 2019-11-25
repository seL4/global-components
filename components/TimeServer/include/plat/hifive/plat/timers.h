/*
 * Copyright 2019, Data61
 * Copyright 2019, DornerWorks
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_DORNERWORKS_BSD)
 */
#pragma once

#define HARDWARE_TIMER_INTERFACES                                                   \
    consumes Dummy pwm0_timer;                                                      \
    consumes Dummy pwm1_timer;                                                      \
    emits Dummy dummy_source;

#define HARDWARE_TIMER_ATTRIBUTES

#define HARDWARE_TIMER_COMPOSITION                                                        \
        connection seL4DTBHardware pwm_timer_conn0(from dummy_source, to pwm0_timer);     \
        connection seL4DTBHardware pwm_timer_conn1(from dummy_source, to pwm1_timer);

#define HARDWARE_TIMER_CONFIG                                                             \
        pwm0_timer.dtb = dtb({"path" : "/soc/pwm@10020000"});                             \
        pwm0_timer.generate_interrupts = 1;                                               \
        pwm1_timer.dtb = dtb({"path" : "/soc/pwm@10021000"});                             \
        pwm1_timer.generate_interrupts = 1;

#define HARDWARE_TIMER_PLAT_INTERFACES
