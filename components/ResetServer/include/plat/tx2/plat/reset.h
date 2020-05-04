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
#include <camkes-BPMPServer.h>


#define HARDWARE_RESET_EXTRA_IMPORTS \
    import <BPMP.idl4>;

#define HARDWARE_RESET_INTERFACES

/* This should be connected to the respective interface in BPMPServer using the */
/* seL4RPCDataport connector. The BPMPServer's interface should be on the 'to' side. */
#define HARDWARE_RESET_EXTRA_INTERFACES \
    BPMPServer_client_interfaces(bpmp)

#define HARDWARE_RESET_ATTRIBUTES

#define HARDWARE_RESET_COMPOSITION

#define HARDWARE_RESET_CONFIG
