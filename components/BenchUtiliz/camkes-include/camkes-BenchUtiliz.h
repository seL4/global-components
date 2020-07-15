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

import <BenchUtiliz/BenchUtiliz.camkes>;

#define BenchUtiliz_control_interfaces(name) \
    uses IdleThread name;

#define BenchUtiliz_control_connections(client_name, client, server) \
    connection seL4RPCCall client##_##client_name##_##server##_control(from client.client_name, to server.idle);
