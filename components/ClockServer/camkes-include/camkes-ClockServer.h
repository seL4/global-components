/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#define _VAR_STRINGIZE(...) #__VA_ARGS__
#define VAR_STRINGIZE(...) _VAR_STRINGIZE(__VA_ARGS__)

import <ClockServerInterfaces.camkes>;


#define ClockServer_client_interfaces(name) \
    uses Clock name; \
    emits Init name##_init1; \
    consumes Init name##_init2;


#define ClockServer_server_interfaces(name) \
    provides Clock name; \
    emits Init name##_init1; \
    consumes Init name##_init2;


#define ClockServer_server_connections(name) \
        connection ClockServerInit name##_server_init(from name##_init1, to name##_init2);

#define ClockServer_client_connections(client_name, client, server_name, server) \
    connection seL4RPCNoThreads client##_##client_name##_##server##_##server_name##_clock(from client.client_name, to server.server_name); \
    connection ClockServerClientInit client##_##client_name##_client_init(from client.client_name##_init1, to client.client_name##_init2);

#define ClockServer_client_connections_embedded(client_name, server_name, server) \
    connection seL4RPCNoThreads client_name##_##server##_##server_name##_clock(from client_name, to server.server_name); \
    connection ClockServerClientInit client_name##_client_init(from client_name##_init1, to client_name##_init2);


#define ClockServer_server_configurations(name) \
    name##_server_init.connection_name = VAR_STRINGIZE(name);

#define ClockServer_client_configurations(client_name, client) \
    client##_##client_name##_client_init.connection_name = VAR_STRINGIZE(client_name);

#define ClockServer_client_configurations_embedded(client_name) \
    client_name##_client_init.connection_name = VAR_STRINGIZE(client_name);


