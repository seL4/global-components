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

import <picotcp-socket-sync.camkes>;


#define picotcp_socket_sync_client_interfaces(name) \
    uses PicoControl name##_control; \
    uses PicoRecv name##_recv; \
    uses PicoSend name##_send; \
    emits Init name##_init1; \
    consumes Init name##_init2;



#define picotcp_socket_sync_server_interfaces(name) \
    provides PicoControl name##_control; \
    provides PicoRecv name##_recv; \
    provides PicoSend name##_send; \
    emits Init name##_init1; \
    consumes Init name##_init2;


#define picotcp_socket_sync_server_connections(name) \
    connection PicotcpSocketSyncServerInit name##_server_init(from name##_init1, to name##_init2);

#define picotcp_socket_sync_client_connections(client, client_name, server, server_name) \
    connection seL4PicoServerSignal client##_##client_name##server##_##server_name##_control(from client.client_name##_control, to server.server_name##_control); \
    connection seL4PicoServer client##_##client_name##server##_##server_name##_recv(from client.client_name##_recv, to server.server_name##_recv); \
    connection seL4PicoServer client##_##client_name##server##_##server_name##_send(from client.client_name##_send, to server.server_name##_send); \
    connection PicotcpSocketSyncClientInit client##_##client_name##_client_init(from client.client_name##_init1, to client.client_name##_init2); \


#define picotcp_socket_sync_server_configurations(name) \
    name##_server_init.connection_name = VAR_STRINGIZE(name);

#define picotcp_socket_sync_client_configurations(client, client_name, packet_size) \
    client.client_name##_recv_shmem_size = packet_size; \
    client.client_name##_send_shmem_size = packet_size; \
    client##_##client_name##_client_init.connection_name = VAR_STRINGIZE(client_name);



