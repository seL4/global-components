/*
 * Copyright 2017, Data61
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

#define HARDWARE_TIMER_INTERFACES                                                   \
    dataport Buf epit2_mem;                                                         \
    consumes DataAvailable epit2_irq;                                               \
	dataport Buf gpt_mem;                                                           \
	consumes DataAvailable gpt_irq;
#define HARDWARE_TIMER_ATTRIBUTES
#define HARDWARE_TIMER_COMPOSITION                                                  \
        component EPIT2 epit2;                                                      \
        connection seL4HardwareMMIO epit2_mem(from epit2_mem, to epit2.mem);        \
        connection seL4HardwareInterrupt epit2_irq(from epit2.irq, to epit2_irq);   \
		component GPT gpt;                                                          \
        connection seL4HardwareMMIO gpt_mem(from gpt_mem, to gpt.mem);              \
        connection seL4HardwareInterrupt gpt_irq(from gpt.irq, to gpt_irq);
#define HARDWARE_TIMER_CONFIG 												        \
		epit2.mem_paddr <- mem_paddr;                                               \
		epit2.mem_size  <-  mem_size;                                               \
		epit2.irq_irq_number <- irq_irq_number;                                     \
		gpt.mem_paddr <- mem_paddr;                                                 \
        gpt.mem_size  <-  mem_size;                                                 \
        gpt.irq_irq_number <- irq_irq_number;
