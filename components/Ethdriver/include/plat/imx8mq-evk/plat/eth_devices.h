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
    consumes Dummy gpio1;                                                           \
    emits Dummy dummy_source; \
    fdt_bind_drivers_interfaces(["/ethernet@30be0000"]);

#define HARDWARE_ETHERNET_COMPOSITION                                               \
    connection seL4DTBHardwareThreadless ethernet_conn(from dummy_source, to EthDriver);      \
    connection seL4DTBHardwareThreadless ocotp_conn(from dummy_source, to ocotp);             \
    connection seL4DTBHardwareThreadless iomux_conn(from dummy_source, to iomux);             \
    connection seL4DTBHardwareThreadless ccm_conn(from dummy_source, to ccm);                 \
    connection seL4DTBHardwareThreadless analog_conn(from dummy_source, to analog);           \
    connection seL4DTBHardwareThreadless gpio1_conn(from dummy_source, to gpio1); \
    fdt_bind_driver_connections();

#define HARDWARE_ETHERNET_CONFIG                                                    \
    EthDriver.dtb = dtb({ "path" : "/ethernet@30be0000" });     \
    EthDriver.generate_interrupts = 1;                                              \
    ocotp.dtb = dtb({ "path" : "/ocotp-ctrl@30350000" });            \
    iomux.dtb = dtb({ "path" : "/iomuxc@30330000" });           \
    ccm.dtb = dtb({ "path" : "/ccm@30380000" });                \
    analog.dtb = dtb({ "path" : "/anatop@30360000" });          \
    gpio1.dtb = dtb({"path" : "/gpio@30200000"});
