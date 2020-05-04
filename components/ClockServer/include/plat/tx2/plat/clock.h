/*
 * Copyright 2019, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(DATA61_GPL)
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
