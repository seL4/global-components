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

#define HARDWARE_CLOCK_EXTRA_IMPORTS

#define HARDWARE_CLOCK_COMPONENT        \
    component BPMP {                    \
        hardware;                       \
        dataport Buf(4096) tx_channel;  \
        dataport Buf(4096) rx_channel;  \
    }

#define HARDWARE_CLOCK_INTERFACES   \
    emits Dummy dummy_source;       \
    consumes Dummy car;             \
    consumes Dummy hsp;             \
    dataport Buf(4096) bpmp_tx;     \
    dataport Buf(4096) bpmp_rx;

#define HARDWARE_CLOCK_EXTRA_INTERFACES

#define HARDWARE_CLOCK_ATTRIBUTES

#define HARDWARE_CLOCK_COMPOSITION  \
    component BPMP bpmp;                                                        \
    connection seL4DTBHardware car_conn(from dummy_source, to car);             \
    connection seL4DTBHardware hsp_conn(from dummy_source, to hsp);             \
    connection seL4HardwareMMIO bpmp_tx_mmio(from bpmp_tx, to bpmp.tx_channel); \
    connection seL4HardwareMMIO bpmp_rx_mmio(from bpmp_rx, to bpmp.rx_channel);

#define HARDWARE_CLOCK_CONFIG                           \
    car.dtb = dtb({ "path" : "/clock@5000000" });       \
    hsp.dtb = dtb({ "path" : "/tegra-hsp@3c00000" });   \
    bpmp.tx_channel_paddr = 0x3004e000;                 \
    bpmp.tx_channel_size = 0x1000;                      \
    bpmp.rx_channel_paddr = 0x3004f000;                 \
    bpmp.rx_channel_size = 0x1000;
