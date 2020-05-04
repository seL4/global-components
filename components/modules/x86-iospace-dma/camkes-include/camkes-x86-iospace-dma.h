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

import <x86-iospace-dma.camkes>;

#define x86_iospace_dma_interfaces(name, iospace_config) \
    emits Init name##_init1; \
    consumes Init name##_init2; \
    attribute string iospaces = iospace_config;


#define x86_iospace_dma_connections(name) \
    connection x86IOSpaceDMA name##_init(from name##_init1, to name##_init2);


#define x86_iospace_dma_configuration(name) \
    name##_init.connection_name = VAR_STRINGIZE(name);
