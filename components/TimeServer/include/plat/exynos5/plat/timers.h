/*
 * Copyright 2018, Data61
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
    dataport Buf pwm_mem;                                                           \
    consumes DataAvailable pwm_irq_t0;                                              \
    consumes DataAvailable pwm_irq_t1;                                              \
    consumes DataAvailable pwm_irq_t2;                                              \
    consumes DataAvailable pwm_irq_t3;                                              \
    consumes DataAvailable pwm_irq_t4;
#define HARDWARE_TIMER_ATTRIBUTES
#define HARDWARE_TIMER_COMPOSITION                                                      \
        component PWM pwm;                                                              \
        connection seL4HardwareMMIO pwm_mem(from pwm_mem, to pwm.mem);                  \
        connection seL4HardwareInterrupt pwm_irq_0(from pwm.irq_t0, to pwm_irq_t0);     \
        connection seL4HardwareInterrupt pwm_irq_1(from pwm.irq_t1, to pwm_irq_t1);     \
        connection seL4HardwareInterrupt pwm_irq_2(from pwm.irq_t2, to pwm_irq_t2);     \
        connection seL4HardwareInterrupt pwm_irq_3(from pwm.irq_t3, to pwm_irq_t3);     \
        connection seL4HardwareInterrupt pwm_irq_4(from pwm.irq_t4, to pwm_irq_t4);
#define HARDWARE_TIMER_CONFIG                                                                       \
        pwm.dtb = dtb({"path" : "/soc/pwm@12dd0000"});                                              \
        pwm.mem_paddr <- pwm.dtb["reg"][0];                                                         \
        pwm.mem_size  = 0x1000;                                                                     \
        pwm.irq_t0_irq_number <- pwm.dtb["interrupts"][0*3 + 1];                                    \
        pwm.irq_t0_spi_number <- pwm.dtb["interrupts"][0*3 + 0];                                    \
        pwm.irq_t1_irq_number <- pwm.dtb["interrupts"][1*3 + 1];                                    \
        pwm.irq_t1_spi_number <- pwm.dtb["interrupts"][1*3 + 0];                                    \
        pwm.irq_t2_irq_number <- pwm.dtb["interrupts"][2*3 + 1];                                    \
        pwm.irq_t2_spi_number <- pwm.dtb["interrupts"][2*3 + 0];                                    \
        pwm.irq_t3_irq_number <- pwm.dtb["interrupts"][3*3 + 1];                                    \
        pwm.irq_t3_spi_number <- pwm.dtb["interrupts"][3*3 + 0];                                    \
        pwm.irq_t4_irq_number <- pwm.dtb["interrupts"][4*3 + 1];                                    \
        pwm.irq_t4_spi_number <- pwm.dtb["interrupts"][4*3 + 0];
#define HARDWARE_TIMER_PLAT_INTERFACES
