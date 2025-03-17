/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


#define _VAR_STRINGIZE(...) #__VA_ARGS__
#define VAR_STRINGIZE(...) _VAR_STRINGIZE(__VA_ARGS__)

import <lwip-base.camkes>;


#define lwip_base_interfaces(name) \
    attribute string name##_ip_addr = ""; \
    attribute string name##_multicast_addr; \
    uses Timer name##_timer; \
    emits Init name##_init1; \
    consumes Init name##_init2;


#define lwip_base_connections(instance, name, timeserver_interface) \
    connection LwipBaseInit instance##_##name##_init(from instance.name##_init1, to instance.name##_init2); \
    connection seL4TimeServer instance##_##name##_timer(from instance.name##_timer, to timeserver_interface);


#define lwip_base_configuration(instance, name, ip_addr, multicast_addr) \
    instance##_##name##_init.connection_name = VAR_STRINGIZE(name); \
    instance.name##_ip_addr = ip_addr; \
    instance.name##_multicast_addr = multicast_addr;
