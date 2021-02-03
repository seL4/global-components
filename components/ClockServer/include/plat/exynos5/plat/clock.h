/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */
#pragma once

#define HARDWARE_CLOCK_EXTRA_IMPORTS

#define HARDWARE_CLOCK_COMPONENT

#define HARDWARE_CLOCK_INTERFACES   \
    emits Dummy dummy_source;       \
    consumes Dummy cmu;             \

#define HARDWARE_CLOCK_EXTRA_INTERFACES

#define HARDWARE_CLOCK_ATTRIBUTES

#define HARDWARE_CLOCK_COMPOSITION  \
    connection seL4DTBHardware cmu_conn(from dummy_source, to cmu);         \

#define HARDWARE_CLOCK_CONFIG   \
    cmu.dtb = dtb({ "path" : "/soc/clock-controller@10010000" });
