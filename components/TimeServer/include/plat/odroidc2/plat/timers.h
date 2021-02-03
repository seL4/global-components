/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#pragma once

#define HARDWARE_TIMER_COMPONENT                                       \
    component HWTimer {                                                \
        hardware;                                                      \
        dataport Buf(4096) mem;                                        \
        emits TimerIRQ irq;                                          \
    }

#define HARDWARE_TIMER_INTERFACES                                      \
    dataport Buf(4096) hwtimer_mem;                                   \
    consumes TimerIRQ hwtimer_irq;
#define HARDWARE_TIMER_ATTRIBUTES
#define HARDWARE_TIMER_COMPOSITION                                     \
        component HWTimer hwtimer;                                    \
        connection seL4HardwareMMIO hwtimer_mem(from hwtimer_mem,    \
                                                 to hwtimer.mem);     \
        connection seL4HardwareInterrupt hwtimer_irq(from hwtimer.irq, \
                                                      to hwtimer_irq);
#define HARDWARE_TIMER_CONFIG                                            \
    hwtimer.mem_paddr = 0xc1109000;                                     \
    hwtimer.mem_size = 0x1000;                                          \
    hwtimer.irq_irq_number = 42;                                        \
    hwtimer.irq_irq_type = "arm";                                           \
    hwtimer.irq_irq_trigger = "edge";
#define HARDWARE_TIMER_PLAT_INTERFACES
