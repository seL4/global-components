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
#pragma once

#define HARDWARE_TIMER_COMPONENT                                       \
    component HWTimer {                                                \
        hardware;                                                      \
        emits TimerIRQ irq;                                          \
    }

#define HARDWARE_TIMER_INTERFACES                                      \
    consumes TimerIRQ hwtimer_irq;
#define HARDWARE_TIMER_ATTRIBUTES
#define HARDWARE_TIMER_COMPOSITION                                     \
        component HWTimer hwtimer;                                    \
        connection seL4HardwareInterrupt hwtimer_irq(from hwtimer.irq, \
                                                      to hwtimer_irq);
#define HARDWARE_TIMER_CONFIG                                            \
    hwtimer.irq_irq_number = 30;                                        \
    hwtimer.irq_irq_type = "arm";
#define HARDWARE_TIMER_PLAT_INTERFACES
