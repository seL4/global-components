/*
 * Copyright 2024, Dornerworks
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#pragma once

#define HARDWARE_SERIAL_INTERFACES              \
        dataport Buf(4096) txt_buf;

#define HARDWARE_SERIAL_ATTRIBUTES


#define HARDWARE_SERIAL_COMPOSITION                                             \
        component VGADisplay vga_hw;                                            \
        connection seL4HardwareMMIO vga_text(from txt_buf, to vga_hw.txt_buf);

#define HARDWARE_SERIAL_CONFIG                  \
        vga_hw.txt_buf_paddr = 0xb8000;         \
        vga_hw.txt_buf_size = 0x1000;