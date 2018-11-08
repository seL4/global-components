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
    dataport Buf(4096) ttc0_mem;                                                    \
    consumes DataAvailable ttc0_irq_t1;                                             \
    consumes DataAvailable ttc0_irq_t2;                                             \
    consumes DataAvailable ttc0_irq_t3;                                             \
    dataport Buf(4096) ttc1_mem;                                                    \
	consumes DataAvailable ttc1_irq_t1;                                             \
	consumes DataAvailable ttc1_irq_t2;                                             \
	consumes DataAvailable ttc1_irq_t3;
#define HARDWARE_TIMER_ATTRIBUTES
#define HARDWARE_TIMER_COMPOSITION                                                      \
        component Timer ttc0;                                                           \
        connection seL4HardwareMMIO ttc0_mem(from ttc0_mem, to ttc0.mem);               \
        connection seL4HardwareInterrupt ttc0_irq_t1(from ttc0.irq_t1, to ttc0_irq_t1); \
        connection seL4HardwareInterrupt ttc0_irq_t2(from ttc0.irq_t2, to ttc0_irq_t2); \
        connection seL4HardwareInterrupt ttc0_irq_t3(from ttc0.irq_t3, to ttc0_irq_t3); \
        component Timer ttc1;                                                           \
        connection seL4HardwareMMIO ttc1_mem(from ttc1_mem, to ttc1.mem);               \
        connection seL4HardwareInterrupt ttc1_irq_t1(from ttc1.irq_t1, to ttc1_irq_t1); \
        connection seL4HardwareInterrupt ttc1_irq_t2(from ttc1.irq_t2, to ttc1_irq_t2); \
        connection seL4HardwareInterrupt ttc1_irq_t3(from ttc1.irq_t3, to ttc1_irq_t3);
#define HARDWARE_TIMER_CONFIG                                                       \
        ttc0.dtb = dtb({"path" : "/amba/timer@f8001000"});                          \
        ttc1.dtb = dtb({"path" : "/amba/timer@f8002000"});                          \
        ttc0.mem_paddr <- ttc0.dtb["reg"][0];                                       \
        ttc0.mem_size  <- ttc0.dtb["reg"][1];                                       \
        ttc0.irq_t1_irq_number <- ttc0.dtb["interrupts"][0*3 + 1];                  \
        ttc0.irq_t1_spi_number <- ttc0.dtb["interrupts"][0*3 + 0];                  \
        ttc0.irq_t2_irq_number <- ttc0.dtb["interrupts"][1*3 + 1];                  \
        ttc0.irq_t2_spi_number <- ttc0.dtb["interrupts"][1*3 + 0];                  \
        ttc0.irq_t3_irq_number <- ttc0.dtb["interrupts"][2*3 + 1];                  \
        ttc0.irq_t3_spi_number <- ttc0.dtb["interrupts"][2*3 + 0];                  \
        ttc1.mem_paddr <- ttc1.dtb["reg"][0];                                       \
        ttc1.mem_size  <- ttc1.dtb["reg"][1];                                       \
        ttc1.irq_t1_irq_number <- ttc1.dtb["interrupts"][0*3 + 1];                  \
        ttc1.irq_t1_spi_number <- ttc1.dtb["interrupts"][0*3 + 0];                  \
        ttc1.irq_t2_irq_number <- ttc1.dtb["interrupts"][1*3 + 1];                  \
        ttc1.irq_t2_spi_number <- ttc1.dtb["interrupts"][1*3 + 0];                  \
        ttc1.irq_t3_irq_number <- ttc1.dtb["interrupts"][2*3 + 1];                  \
        ttc1.irq_t3_spi_number <- ttc1.dtb["interrupts"][2*3 + 0];
#define HARDWARE_TIMER_PLAT_INTERFACES
