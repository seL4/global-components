/*
 * Copyright 2019, DornerWorks
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#pragma once

#define HARDWARE_SERIAL_INTERFACES  \
    emits Dummy dummy_source;       \
    consumes Dummy serial_dev;

#define HARDWARE_SERIAL_ATTRIBUTES

#define HARDWARE_SERIAL_COMPOSITION                                                 \
        connection seL4DTBHardware serial_conn(from dummy_source, to serial_dev);

#define HARDWARE_SERIAL_CONFIG                               \
        serial_dev.dtb = dtb({ "chosen" : "stdout-path" });  \
        serial_dev.generate_interrupts = 1;
