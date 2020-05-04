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


import <dynamic-untyped-allocators.camkes>;


#define dynamic_untyped_allocators_interfaces(name) \
    emits Init name##_init1; \
    consumes Init name##_init2;

#define dynamic_untyped_allocators_connections(name) \
    connection DynamicUntypedAllocatorEnvironment name##_init(from name##_init1, to name##_init2);


#define dynamic_untyped_allocators_configuration(name) \
    name##_init.connection_name = VAR_STRINGIZE(name);
