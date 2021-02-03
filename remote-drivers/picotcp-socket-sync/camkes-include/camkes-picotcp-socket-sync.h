/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#define _VAR_STRINGIZE(...) #__VA_ARGS__
#define VAR_STRINGIZE(...) _VAR_STRINGIZE(__VA_ARGS__)

import <picotcp-socket-sync.camkes>;


#define picotcp_socket_sync_client_interfaces(name) \
    uses PicoControl name##_control; \
    uses PicoRecv name##_recv; \
    uses PicoSend name##_send; \
    emits Init name##_init1; \
    uses VirtQueueDrv name##_tx; \
    uses VirtQueueDrv name##_rx; \
    dataport Buf name##_dma_pool; \
    consumes Init name##_init2;



#define picotcp_socket_sync_server_interfaces(name) \
    provides PicoControl name##_control; \
    provides PicoRecv name##_recv; \
    provides PicoSend name##_send; \
    emits Init name##_init1; \
    maybe uses VirtQueueDev name##_tx; \
    maybe uses VirtQueueDev name##_rx; \
    attribute int name##_tx_shmem_size = 8192*4; \
    attribute int name##_rx_shmem_size = 8192*4; \
    maybe dataport Buf name##_dma_pool; \
    consumes Init name##_init2;


#define picotcp_socket_sync_server_connections(name) \
    connection PicotcpSocketSyncServerInit name##_server_init(from name##_init1, to name##_init2);

#define picotcp_socket_sync_client_connections(client, client_name, server, server_name) \
    connection seL4PicoServerSignal client##_##client_name##server##_##server_name##_control(from client.client_name##_control, to server.server_name##_control); \
    connection seL4PicoServer client##_##client_name##server##_##server_name##_recv(from client.client_name##_recv, to server.server_name##_recv); \
    connection seL4PicoServer client##_##client_name##server##_##server_name##_send(from client.client_name##_send, to server.server_name##_send); \
    connection PicotcpSocketSyncClientInit client##_##client_name##_client_init(from client.client_name##_init1, to client.client_name##_init2); \
    component VirtQueueInit client_name##d0; \
    component VirtQueueInit client_name##d1; \
    connection seL4VirtQueues client_name##_virtq_conn0(to client_name##d0.init, from client.client_name##_tx, from server.server_name##_tx); \
    connection seL4VirtQueues client_name##_virtq_conn1(to client_name##d1.init, from client.client_name##_rx, from server.server_name##_rx); \
    connection seL4DMASharedData client_name##_dma(from client.client_name##_dma_pool, to server.server_name##_dma_pool);


#define picotcp_socket_sync_server_configurations(name) \
    name##_server_init.connection_name = VAR_STRINGIZE(name);

#define picotcp_socket_sync_client_configurations(client, client_name, packet_size, q_length, async_pool_size) \
    client.client_name##_recv_shmem_size = packet_size; \
    client.client_name##_send_shmem_size = packet_size; \
    client##_##client_name##_client_init.connection_name = VAR_STRINGIZE(client_name); \
    client_name##_dma.size = async_pool_size; \
    client_name##_virtq_conn0.queue_length = q_length; \
    client_name##_virtq_conn1.queue_length = q_length; \
    client.client_name##_tx_shmem_size = 8192*4; \
    client.client_name##_rx_shmem_size = 8192*4; \
    client_name##_dma.controller = VAR_STRINGIZE(client.client_name##_dma_pool);




