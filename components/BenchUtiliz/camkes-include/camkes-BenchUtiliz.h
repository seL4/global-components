/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#define _VAR_STRINGIZE(...) #__VA_ARGS__
#define VAR_STRINGIZE(...) _VAR_STRINGIZE(__VA_ARGS__)

import <BenchUtiliz/BenchUtiliz.camkes>;


#define BenchUtiliz_trace_interfaces_maybe(name) \
    maybe consumes Trace name##_start; \
    maybe consumes Trace name##_stop;

#define BenchUtiliz_control_interfaces(name) \
    uses IdleThread name;


#define BenchUtiliz_trace_connections(client_name, client, server) \
    connection seL4Notification client##_##client_name##_##server##_start(from server.trace_start, to client.client_name##_start); \
    connection seL4Notification client##_##client_name##_##server##_stop(from server.trace_stop, to client.client_name##_stop);

#define BenchUtiliz_control_connections(client_name, client, server) \
    connection seL4RPCCall client##_##client_name##_##server##_control(from client.client_name, to server.idle);


#define BenchUtiliz_trace_configurations(client_name, client, priority) \
    client.client_name##_stop_priority = priority; \
    client.client_name##_start_priority = priority;
