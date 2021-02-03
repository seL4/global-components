<!--
     Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)

     SPDX-License-Identifier: CC-BY-SA-4.0
-->

# BPMPServer

This is a CAmkES component that is intended to abstract over the TX2's Boot and
Power Management Processor (BPMP) co-processor and multiplex client access to
it.

## Dependencies

This component needs to be used with the `single-threaded` module that performs
the initialisation process, and client request routing the component relies on.

## Usage

For clients to use the functionality exposed by the component, the clients will
have to connect to the CAmkES interface that the following CPP defintion
exposes.

```c
BPMPServer_server_interfaces(the_bpmp)
```

The definition exposes a `BPMP` RPC interface that is expected to be connected
via the `seL4RPCNoThreads` connector. However, there is a helper macro which
simplifies this process and also registers a BPMP interface on the client's
interface registration service (`ps_interface_ops_t`).

```c
BPMPServer_client_connections(client_name, client, server_name, server)
```

## Supported platforms

The component only supports the TX2 platform as the device is exclusive to the TX2.

## Limitations

There are currently no limitations with this component.
