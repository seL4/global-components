/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#define _VAR_STRINGIZE(...) #__VA_ARGS__
#define VAR_STRINGIZE(...) _VAR_STRINGIZE(__VA_ARGS__)

import <picotcp-ethernet-async.camkes>;


#define picotcp_ethernet_async_client_interfaces(name) \
    uses picotcp_ethernet_async_control name##_control; \
    dataport Buf name##_dma_pool; \
    uses VirtQueueDrv name##_tx; \
    uses VirtQueueDrv name##_rx; \
    emits Init name##_init1; \
    consumes Init name##_init2;


#define picotcp_ethernet_async_server_interfaces(name) \
    provides picotcp_ethernet_async_control name##_control; \
    dataport Buf name##_dma_pool; \
    uses VirtQueueDev name##_tx; \
    uses VirtQueueDev name##_rx; \
    emits Init name##_init1; \
    consumes Init name##_init2;



#define picotcp_ethernet_async_connections(name, client, driver) \
    component VirtQueueInit name##d0; \
    component VirtQueueInit name##d1; \
    connection seL4RPCNoThreads name##_eth_driver_conn(from client.name##_control, to driver.name##_control); \
    connection seL4VirtQueues name##_virtq_conn0(to name##d0.init, from client.name##_tx, from driver.name##_tx); \
    connection seL4VirtQueues name##_virtq_conn1(to name##d1.init, from client.name##_rx, from driver.name##_rx); \
    connection seL4DMASharedData name##_dma(from client.name##_dma_pool, to driver.name##_dma_pool); \
    connection PicotcpEthernetAsyncClientInit name##_client_init(from client.name##_init1, to client.name##_init2); \
    connection PicotcpEthernetAsyncServerInit name##_server_init(from driver.name##_init1, to driver.name##_init2);


#define picotcp_ethernet_async_configurations(name, client, driver) \
    name##_dma.size = 0x100000; \
    name##_dma.controller = VAR_STRINGIZE(client.name##_dma_pool); \
    name##_server_init.connection_name = VAR_STRINGIZE(name); \
    name##_client_init.connection_name = VAR_STRINGIZE(name); \
    name##_virtq_conn0.queue_length = 512; \
    name##_virtq_conn1.queue_length = 512; \
    client.name##_tx_shmem_size = 8192*4; \
    client.name##_rx_shmem_size = 8192*4; \
    driver.name##_tx_shmem_size = 8192*4; \
    driver.name##_rx_shmem_size = 8192*4;

#define picotcp_ethernet_async_configurations_uncached_dma(name, client, driver) \
    client.name##_dma_pool_cached = false; \
    driver.name##_dma_pool_cached = false;

