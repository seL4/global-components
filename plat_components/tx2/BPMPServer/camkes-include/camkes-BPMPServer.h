/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#define _VAR_STRINGIZE(...) #__VA_ARGS__
#define VAR_STRINGIZE(...) _VAR_STRINGIZE(__VA_ARGS__)

import <BPMPServerInterfaces.camkes>;


#define BPMPServer_client_interfaces(name) \
    uses BPMP name; \
    emits Init name##_init1; \
    consumes Init name##_init2;


#define BPMPServer_server_interfaces(name) \
    provides BPMP name; \
    emits Init name##_init1; \
    consumes Init name##_init2;


#define BPMPServer_server_connections(name) \
        connection BPMPServerInit name##_server_init(from name##_init1, to name##_init2);


#define BPMPServer_client_connections(client_name, client, server_name, server) \
    connection seL4RPCDataportNoThreads client##_##client_name##_##server##_##server_name##_bpmp(from client.client_name, to server.server_name); \
    connection BPMPServerClientInit client##_##client_name##_client_init(from client.client_name##_init1, to client.client_name##_init2);

#define BPMPServer_client_connections_embedded(client_name, server_name, server) \
    connection seL4RPCDataportNoThreads client_name##_##server##_##server_name##_bpmp(from client_name, to server.server_name); \
    connection BPMPServerClientInit client_name##_client_init(from client_name##_init1, to client_name##_init2);


#define BPMPServer_server_configurations(name) \
    name##_server_init.connection_name = VAR_STRINGIZE(name);

#define BPMPServer_client_configurations(client_name, client) \
    client##_##client_name##_client_init.connection_name = VAR_STRINGIZE(client_name);

#define BPMPServer_client_configurations_embedded(client_name) \
    client_name##_client_init.connection_name = VAR_STRINGIZE(client_name);
