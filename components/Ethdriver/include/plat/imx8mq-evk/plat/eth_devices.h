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

#define HARDWARE_ETHERNET_EXTRA_IMPORTS

#define HARDWARE_ETHERNET_COMPONENT

#define HARDWARE_ETHERNET_INTERFACES                                                \
    consumes Dummy EthDriver;                                                       \
    consumes Dummy ocotp;                                                           \
    consumes Dummy iomux;                                                           \
    consumes Dummy ccm;                                                             \
    consumes Dummy analog;                                                          \
    consumes Dummy gpio1;                                                           \
    emits Dummy dummy_source;

#define HARDWARE_ETHERNET_COMPOSITION                                               \
    connection seL4DTBHardware ethernet_conn(from dummy_source, to EthDriver);      \
    connection seL4DTBHardware ocotp_conn(from dummy_source, to ocotp);             \
    connection seL4DTBHardware iomux_conn(from dummy_source, to iomux);             \
    connection seL4DTBHardware ccm_conn(from dummy_source, to ccm);                 \
    connection seL4DTBHardware analog_conn(from dummy_source, to analog);           \
    connection seL4DTBHardware gpio1_conn(from dummy_source, to gpio1);

#define HARDWARE_ETHERNET_CONFIG                                                    \
    EthDriver.dtb = dtb({ "path" : "/ethernet@30be0000" });     \
    EthDriver.generate_interrupts = 1;                                              \
    ocotp.dtb = dtb({ "path" : "/ocotp-ctrl@30350000" });            \
    iomux.dtb = dtb({ "path" : "/iomuxc@30330000" });           \
    ccm.dtb = dtb({ "path" : "/ccm@30380000" });                \
    analog.dtb = dtb({ "path" : "/anatop@30360000" });          \
    gpio1.dtb = dtb({"path" : "/gpio@30200000"});

