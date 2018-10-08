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

#define HARDWARE_SERIAL_INTERFACES                              \
    dataport Buf serial_mem;                                    \
    consumes DataAvailable serial_irq;

#define HARDWARE_SERIAL_ATTRIBUTES

#define HARDWARE_SERIAL_COMPOSITION                                                     \
        component UART serial;                                                          \
        connection seL4HardwareMMIO serial_mem(from serial_mem, to serial.mem);         \
        connection seL4HardwareInterrupt serial_irq(from serial.irq, to serial_irq);

#define HARDWARE_SERIAL_CONFIG                                  \
        serial.dtb = dtb({"path":"/soc/serial@12c20000"});      \
        serial.mem_paddr <- serial.dtb["reg"][0];               \
        serial.mem_size = 0x1000;                               \
        serial.irq_irq_number <- serial.dtb["interrupts"][1];   \
        serial.irq_spi_number <- serial.dtb["interrupts"][0];   \
        serial.irq_export_irq = 1;
