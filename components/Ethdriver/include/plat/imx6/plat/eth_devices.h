/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */
#pragma once
#include <camkes-fdt-bind-driver.h>

#define HARDWARE_ETHERNET_EXTRA_IMPORTS

#define HARDWARE_ETHERNET_COMPONENT

#define HARDWARE_ETHERNET_INTERFACES                                                \
    consumes Dummy EthDriver;                                                       \
    consumes Dummy ocotp;                                                           \
    consumes Dummy iomux;                                                           \
    consumes Dummy ccm;                                                             \
    consumes Dummy analog;                                                          \
    consumes Dummy gpio3;                                                           \
    consumes Dummy gpio6;                                                           \
    emits Dummy dummy_source; \
    fdt_bind_drivers_interfaces(["/soc/aips-bus@2100000/ethernet@2188000"]);

#define HARDWARE_ETHERNET_COMPOSITION                                               \
    connection seL4DTBHardwareThreadless ethernet_conn(from dummy_source, to EthDriver);      \
    connection seL4DTBHardwareThreadless ocotp_conn(from dummy_source, to ocotp);             \
    connection seL4DTBHardwareThreadless iomux_conn(from dummy_source, to iomux);             \
    connection seL4DTBHardwareThreadless ccm_conn(from dummy_source, to ccm);                 \
    connection seL4DTBHardwareThreadless analog_conn(from dummy_source, to analog);           \
    connection seL4DTBHardwareThreadless gpio3_conn(from dummy_source, to gpio3);             \
    connection seL4DTBHardwareThreadless gpio6_conn(from dummy_source, to gpio6); \
    fdt_bind_driver_connections();

#define HARDWARE_ETHERNET_CONFIG                                                    \
    EthDriver.dtb = dtb({ "path" : "/soc/aips-bus@2100000/ethernet@2188000" });     \
    EthDriver.generate_interrupts = 1;                                              \
    ocotp.dtb = dtb({ "path" : "/soc/aips-bus@2100000/ocotp@21bc000" });            \
    iomux.dtb = dtb({ "path" : "/soc/aips-bus@2000000/iomuxc@20e0000" });           \
    ccm.dtb = dtb({ "path" : "/soc/aips-bus@2000000/ccm@20c4000" });                \
    analog.dtb = dtb({ "path" : "/soc/aips-bus@2000000/anatop@20c8000" });          \
    gpio3.dtb = dtb({ "path" : "/soc/aips-bus@2000000/gpio@20a4000" });             \
    gpio6.dtb = dtb({ "path" : "/soc/aips-bus@2000000/gpio@20b0000" });
