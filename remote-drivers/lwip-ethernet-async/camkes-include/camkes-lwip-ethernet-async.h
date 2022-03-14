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

import <std_connector.camkes>;
import <global-connectors.camkes>;
import <lwip-ethernet-async.camkes>;

#define lwip_ethernet_async_client_interfaces(name) \
    uses lwip_ethernet_async_control name##_control; \
    dataport Buf name##_dma_pool; \
    include "shared_ringbuffer/shared_ringbuffer.h"; \
    dataport ring_buffer_t rx_avail; \
    dataport ring_buffer_t rx_used; \
    dataport ring_buffer_t tx_avail; \
    dataport ring_buffer_t tx_used; \
    consumes Notification rx_done; \
    emits Notification rx_empty; \
    emits Notification tx_ready; \
    emits Init name##_init1; \
    consumes Init name##_init2;

#define lwip_ethernet_async_server_interfaces(name) \
    provides lwip_ethernet_async_control name##_control; \
    dataport Buf name##_dma_pool; \
    include "shared_ringbuffer/shared_ringbuffer.h"; \
    dataport ring_buffer_t rx_avail; \
    dataport ring_buffer_t rx_used; \
    dataport ring_buffer_t tx_avail; \
    dataport ring_buffer_t tx_used; \
    emits Notification rx_done; \
    consumes Notification rx_empty; \
    consumes Notification tx_ready; \
    emits Init name##_init1; \
    consumes Init name##_init2;

#define lwip_ethernet_async_connections(name, client, driver) \
    connection seL4RPCNoThreads name##_eth_driver_conn(from client.name##_control, to driver.name##_control); \
    connection seL4SharedData rx_avail(from client.rx_avail, to driver.rx_avail); \
    connection seL4SharedData rx_used(from client.rx_used, to driver.rx_used); \    
    connection seL4SharedData tx_avail(from client.tx_avail, to driver.tx_avail); \
    connection seL4SharedData tx_used(from client.tx_used, to driver.tx_used); \ 
    connection seL4Notification tx_ready(from client.tx_ready, to driver.tx_ready); \
    connection seL4Notification rx_done(from driver.rx_done, to client.rx_done); \
    connection seL4Notification rx_empty(from client.rx_empty, to driver.rx_empty); \
    connection seL4DMASharedData name##_dma(from client.name##_dma_pool, to driver.name##_dma_pool); \
    connection LwipEthernetAsyncClientInit name##_client_init(from client.name##_init1, to client.name##_init2); \
    connection LwipEthernetAsyncServerInit name##_server_init(from driver.name##_init1, to driver.name##_init2);

#define lwip_ethernet_async_configurations(name, client, driver) \
    name##_dma.size = 0x400000; \
    name##_dma.controller = VAR_STRINGIZE(client.name##_dma_pool); \
    name##_server_init.connection_name = VAR_STRINGIZE(name); \
    name##_client_init.connection_name = VAR_STRINGIZE(name); \
    rx_avail.size = 0x15000; \
    rx_used.size = 0x15000; \
    tx_avail.size = 0x15000; \
    tx_used.size = 0x15000; \
    client.tx_ready_priority = 100; \
    driver.tx_ready_priority = 100; \
    client.rx_done_priority = 100; \
    driver.rx_done_priority = 100; \
    client.rx_empty_priority = 100; \
    driver.rx_empty_priority = 100;
