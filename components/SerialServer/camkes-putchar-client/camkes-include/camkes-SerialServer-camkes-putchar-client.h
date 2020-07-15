/*
 * Copyright 2020, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
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
