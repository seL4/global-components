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
