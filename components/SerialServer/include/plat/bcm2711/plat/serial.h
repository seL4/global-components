/*
 * Copyright 2022, Technology Innovation Institute
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#pragma once

#define HARDWARE_SERIAL_COMPONENT                                      \
    component GPIOHW {                                                 \
        hardware;                                                      \
        dataport Buf(4096) mem;                                        \
    }

#define HARDWARE_SERIAL_INTERFACES  \
    dataport Buf(4096) gpiohw_mem;  \
    emits Dummy dummy_source;       \
    consumes Dummy serial_dev;

#define HARDWARE_SERIAL_ATTRIBUTES

#define HARDWARE_SERIAL_COMPOSITION                                                 \
        component GPIOHW gpio_hw;                                                   \
        connection seL4DTBHardware serial_conn(from dummy_source, to serial_dev);   \
        connection seL4HardwareMMIO gpio_mem(from gpiohw_mem, to gpio_hw.mem);

#define HARDWARE_SERIAL_CONFIG                               \
        serial_dev.dtb = dtb({ "chosen" : "stdout-path" });  \
        serial_dev.generate_interrupts = 1;                  \
        gpio_hw.mem_paddr = 0xfe200000;                      \
        gpio_hw.mem_size = 0x1000;
