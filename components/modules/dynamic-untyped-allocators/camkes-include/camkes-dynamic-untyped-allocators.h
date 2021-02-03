/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#define _VAR_STRINGIZE(...) #__VA_ARGS__
#define VAR_STRINGIZE(...) _VAR_STRINGIZE(__VA_ARGS__)


import <dynamic-untyped-allocators.camkes>;


#define dynamic_untyped_allocators_interfaces(name) \
    emits Init name##_init1; \
    consumes Init name##_init2;

#define dynamic_untyped_allocators_connections(name) \
    connection DynamicUntypedAllocatorEnvironment name##_init(from name##_init1, to name##_init2);


#define dynamic_untyped_allocators_configuration(name) \
    name##_init.connection_name = VAR_STRINGIZE(name);
