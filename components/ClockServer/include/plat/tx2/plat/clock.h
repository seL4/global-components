/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */
#pragma once

#include <camkes-BPMPServer.h>

#define HARDWARE_CLOCK_EXTRA_IMPORTS \
    import <BPMP.idl4>;

#define HARDWARE_CLOCK_COMPONENT

#define HARDWARE_CLOCK_INTERFACES   \
    emits Dummy dummy_source;       \
    consumes Dummy car;             \

/* Connect the bpmp 'uses' interface to the BPMPServer component using the
 * seL4RPCDataport connector. The BPMPServer's interface should be on the 'to'
 * side. */
#define HARDWARE_CLOCK_EXTRA_INTERFACES \
    BPMPServer_client_interfaces(bpmp)

#define HARDWARE_CLOCK_ATTRIBUTES

#define HARDWARE_CLOCK_COMPOSITION  \
    connection seL4DTBHardware car_conn(from dummy_source, to car);

#define HARDWARE_CLOCK_CONFIG                           \
    car.dtb = dtb({ "path" : "/clock@5000000" });
