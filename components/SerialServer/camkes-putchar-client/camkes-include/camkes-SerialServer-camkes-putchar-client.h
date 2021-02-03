/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#define _VAR_STRINGIZE(...) #__VA_ARGS__
#define VAR_STRINGIZE(...) _VAR_STRINGIZE(__VA_ARGS__)


import <SerialServer-camkes-putchar-client.camkes>;

#define SerialServer_putchar_printf_client(name) \
    uses PutChar name##_putchar; \
    emits Init name; \
    consumes Init name##_init2;


#define SerialServer_raw_putchar_printf_connection(name, client, driver) \
    connection seL4RPCCall name##_##client##_##driver##_putchar(from client.name##_putchar, to driver.raw_putchar); \
    connection SerialServerCamkesPutcharClient name##_##client##_##driver##_init(from client.name, to client.name##_init2);

#define SerialServer_processed_putchar_printf_connection(name, client, driver) \
    connection seL4RPCCall name##_##client##_##driver##_putchar(from client.name##_putchar, to driver.processed_putchar); \
    connection SerialServerCamkesPutcharClient name##_##client##_##driver##_init(from client.name, to client.name##_init2);
