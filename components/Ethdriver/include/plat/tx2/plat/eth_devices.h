/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */
#pragma once

#include <camkes-BPMPServer.h>
#include <camkes-ClockServer.h>
#include <camkes-ResetServer.h>
#include <camkes-GPIOServer.h>
#include <camkes-fdt-bind-driver.h>

#define HARDWARE_ETHERNET_EXTRA_IMPORTS             \
    import <BPMP.idl4>;                             \
    import <BPMPServer/BPMPServer.camkes>;          \
    import <ClockServer/ClockServer.camkes>;        \
    import <ResetServer/ResetServer.camkes>;        \
    import <GPIOMUXServer/GPIOMUXServer.camkes>;

#define HARDWARE_ETHERNET_COMPONENT

#define HARDWARE_ETHERNET_INTERFACES    \
    consumes Dummy EthDriver;           \
    emits Dummy dummy_source;           \
    ClockServer_client_interfaces(clock)\
    ResetServer_client_interfaces(reset)\
    GPIOServer_client_interfaces(gpio)\
    fdt_bind_drivers_interfaces(["/ether_qos@2490000"]); \
    attribute int dma_pool_paddr = 0x90000000;


#define HARDWARE_ETHERNET_COMPOSITION                                                   \
    component BPMPServer bpmp_server;                                                   \
    component ClockServer clock_server;                                                 \
    component ResetServer reset_server;                                                 \
    component GPIOMUXServer gpiomux_server;                                             \
    BPMPServer_client_connections(bpmp, clock_server, the_bpmp, bpmp_server) \
    BPMPServer_client_connections(bpmp, reset_server, the_bpmp, bpmp_server) \
    ClockServer_client_connections_embedded(clock, the_clock, clock_server) \
    ResetServer_client_connections_embedded(reset, the_reset, reset_server) \
    GPIOServer_client_connections_embedded(gpio, the_gpio, gpiomux_server) \
    connection seL4DTBHardwareThreadless ethdriver_conn(from dummy_source, to EthDriver);         \
    fdt_bind_driver_connections();



#define HARDWARE_ETHERNET_CONFIG                                \
    EthDriver.dtb = dtb({ "path" : "/ether_qos@2490000" });     \
    EthDriver.generate_interrupts = 1; \
    BPMPServer_client_configurations(bpmp, clock_server) \
    BPMPServer_client_configurations(bpmp, reset_server) \
    ClockServer_client_configurations_embedded(clock) \
    ResetServer_client_configurations_embedded(reset) \
    GPIOServer_client_configurations_embedded(gpio)
