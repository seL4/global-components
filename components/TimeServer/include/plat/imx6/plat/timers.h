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
    dataport Buf(16384) epit2_mem;                                                  \
    consumes DataAvailable epit2_irq;                                               \
	dataport Buf(16384) gpt_mem;                                                    \
	consumes DataAvailable gpt_irq;
#define HARDWARE_TIMER_ATTRIBUTES
#define HARDWARE_TIMER_COMPOSITION                                                  \
        component EPIT epit2;                                                      \
        connection seL4HardwareMMIO epit2_mem(from epit2_mem, to epit2.mem);        \
        connection seL4HardwareInterrupt epit2_irq(from epit2.irq, to epit2_irq);   \
        component GPT gpt;                                                          \
        connection seL4HardwareMMIO gpt_mem(from gpt_mem, to gpt.mem);              \
        connection seL4HardwareInterrupt gpt_irq(from gpt.irq, to gpt_irq);
#define HARDWARE_TIMER_CONFIG                                                                       \
        epit2.dtb = dtb({"path" : "/soc/aips-bus@2000000/epit@20d4000"});                           \
        gpt.dtb = dtb({"path" : "/soc/aips-bus@2000000/gpt@2098000"});                              \
        epit2.mem_paddr <- epit2.dtb["reg"][0];                                                     \
        epit2.mem_size  <- epit2.dtb["reg"][1];                                                     \
        epit2.irq_irq_number <- epit2.dtb["interrupts"][1];                                         \
        epit2.irq_spi_number <- epit2.dtb["interrupts"][0];                                             \
        gpt.mem_paddr <- gpt.dtb["reg"][0];                                                         \
        gpt.mem_size  <-  gpt.dtb["reg"][1];                                                        \
        gpt.irq_irq_number <- gpt.dtb["interrupts"][1];                                             \
        gpt.irq_spi_number <- gpt.dtb["interrupts"][0];
#define HARDWARE_TIMER_PLAT_INTERFACES
