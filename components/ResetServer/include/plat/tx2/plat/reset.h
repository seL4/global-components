/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
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
