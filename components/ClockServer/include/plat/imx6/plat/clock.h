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
    consumes Dummy ccm;             \
    consumes Dummy anatop;

#define HARDWARE_CLOCK_EXTRA_INTERFACES

#define HARDWARE_CLOCK_ATTRIBUTES

#define HARDWARE_CLOCK_COMPOSITION  \
    connection seL4DTBHardware ccm_conn(from dummy_source, to ccm);         \
    connection seL4DTBHardware anatop_conn(from dummy_source, to anatop);

#define HARDWARE_CLOCK_CONFIG   \
    ccm.dtb = dtb({ "path" : "/soc/aips-bus@2000000/ccm@20c4000" });        \
    anatop.dtb = dtb({ "path" : "/soc/aips-bus@2000000/anatop@20c8000" });
