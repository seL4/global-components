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



#define HARDWARE_ETHERNET_COMPONENT     \
    component BPMP {                    \
        hardware;                       \
        dataport Buf(4096) tx_channel;  \
        dataport Buf(4096) rx_channel;  \
    }

#define HARDWARE_ETHERNET_INTERFACES                                    \
    consumes Dummy EthDriver;                                           \
    consumes Dummy Clock;               \
    consumes Dummy Hsp;                 \
    consumes Dummy Gpio;                \
    consumes Dummy mux;                \
    dataport Buf(4096) bpmp_tx;         \
    dataport Buf(4096) bpmp_rx;         \
    emits Dummy dummy_source;


#define HARDWARE_ETHERNET_COMPOSITION                                           \
    component BPMP bpmp;                                                        \
    connection seL4HardwareMMIO bpmp_tx_mmio(from bpmp_tx, to bpmp.tx_channel); \
    connection seL4HardwareMMIO bpmp_rx_mmio(from bpmp_rx, to bpmp.rx_channel); \
    connection seL4DTBHardware hsp_conn(from dummy_source, to Hsp);             \
    connection seL4DTBHardware ethdriver_conn(from dummy_source, to EthDriver); \
    connection seL4DTBHardware clock_conn(from dummy_source, to Clock);         \
    connection seL4DTBHardware mux_conn(from dummy_source, to mux);             \
    connection seL4DTBHardware gpio_conn(from dummy_source, to Gpio);



#define HARDWARE_ETHERNET_CONFIG                                        \
    Clock.dtb = dtb({ "path" : "/clock@5000000" });                     \
    Hsp.dtb = dtb({ "path" : "/tegra-hsp@3c00000" });                   \
    EthDriver.dtb = dtb({ "path" : "/ether_qos@2490000" });             \
    Gpio.dtb = dtb({ "path" : "/gpio@2200000" });                       \
    mux.dtb = dtb({"path" : "/pinmux@2430000"});                        \
    bpmp.tx_channel_paddr = 0x3004e000;                                 \
    bpmp.tx_channel_size = 0x1000;                                      \
    bpmp.rx_channel_paddr = 0x3004f000;                                 \
    bpmp.rx_channel_size = 0x1000;                                      \
    EthDriver.generate_interrupts = 1;
