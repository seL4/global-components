/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */
#pragma once

#include <camkes-fdt-bind-driver.h>

#define HARDWARE_ETHERNET_EXTRA_IMPORTS

#define HARDWARE_ETHERNET_COMPONENT

#define HARDWARE_ETHERNET_INTERFACES                                    \
    consumes Dummy EthDriver;                                           \
    consumes Dummy slcr;                                                \
    emits Dummy dummy_source; \
    fdt_bind_drivers_interfaces(["/amba/ethernet@e000b000"]);

#define HARDWARE_ETHERNET_COMPOSITION                                   \
    connection seL4DTBHardwareThreadless ethdriver_conn(from dummy_source,        \
                                             to EthDriver);             \
    connection seL4DTBHardwareThreadless slcr_conn(from dummy_source,             \
                                         to slcr); \
    fdt_bind_driver_connections();

#define HARDWARE_ETHERNET_CONFIG                                        \
    EthDriver.dtb = dtb({ "path" : "/amba/ethernet@e000b000" });        \
    EthDriver.generate_interrupts = 1;                                  \
    slcr.dtb = dtb({ "path" : "/amba/slcr@f8000000" });
