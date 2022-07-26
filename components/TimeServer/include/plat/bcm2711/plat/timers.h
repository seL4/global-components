/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 * Copyright 2021, Technology Innovation Institute
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#pragma once

#define HARDWARE_TIMER_COMPONENT                                       \
    component HWTimer {                                                \
        hardware;                                                      \
        dataport Buf(4096) mem;                                        \
        emits TimerIRQ irq;                                            \
    }

#define HARDWARE_TIMER_INTERFACES                                      \
    dataport Buf(4096) systimer_mem;                                   \
    dataport Buf(4096) sp804_mem;                                      \
    consumes TimerIRQ systimer_irq;                                    \
    consumes TimerIRQ sp804_irq;

#define HARDWARE_TIMER_ATTRIBUTES
#define HARDWARE_TIMER_COMPOSITION                                       \
        component HWTimer systimer;                                      \
        connection seL4HardwareMMIO systimer_mem(from systimer_mem,      \
                                                 to systimer.mem);       \
        connection seL4HardwareInterrupt systimer_irq(from systimer.irq, \
                                                      to systimer_irq);  \
        component HWTimer sp804timer;                                    \
        connection seL4HardwareMMIO sp804_mem(from sp804_mem,            \
                                              to sp804timer.mem);        \
        connection seL4HardwareInterrupt sp804_irq(from sp804timer.irq,  \
                                                   to sp804_irq);
#define HARDWARE_TIMER_CONFIG                                            \
    systimer.mem_paddr = 0xfe003000;                                     \
    systimer.mem_size = 0x1000;                                          \
    systimer.irq_irq_number = 97;                                        \
    sp804timer.mem_paddr = 0xfe00b000;                                   \
    sp804timer.mem_size = 0x1000;                                        \
    sp804timer.irq_irq_number = 64;
#define HARDWARE_TIMER_PLAT_INTERFACES
