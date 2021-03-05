/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */
#pragma once
#include <camkes-fdt-bind-driver.h>

#define HARDWARE_ETHERNET_EXTRA_IMPORTS

#define HARDWARE_ETHERNET_COMPONENT

#define HARDWARE_ETHERNET_INTERFACES                            \
    consumes Dummy EthDriver;                                   \
    emits Dummy dummy_source;                                   \
    fdt_bind_drivers_interfaces(["/soc/ethernet@c9410000"]);

#define HARDWARE_ETHERNET_COMPOSITION                                                         \
    connection seL4DTBHardwareThreadless ethernet_conn(from dummy_source, to EthDriver);      \
    fdt_bind_driver_connections();

#define HARDWARE_ETHERNET_CONFIG                                \
    EthDriver.dtb = dtb({ "path" : "/soc/ethernet@c9410000" }); \
    EthDriver.generate_interrupts = 1;
