<!--
     Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)

     SPDX-License-Identifier: CC-BY-SA-4.0
-->

# PicoServer

This is a CAmkES component that is intended to wrap around the PicoTCP TCP/ICP
stack library and multiplex client access to it.

## Dependencies

This component needs to be used with the `single-threaded` module that performs
the initialisation process, and client request routing the component relies on.
The `picotcp_socket_sync` module is an optional dependency which provides
helper CAmkES macros and an asynchronous data transfer interface for clients.

## Usage

For clients to use the functionaity exposed by the component, the clients will
have to connect to the CAmkES interfaces that the following CPP definition
expose.

```c
picotcp_socket_sync_server_interfaces(pico)
```

The definition exposes the `PicoControl`, `PicoSend` and `PicoRecv` RPC
interfaces that is expected to be connected via the `seL4RPCCallSignal`,
`seL4RPCDataport`, and `seL4RPCDataport` connectors respectively. There is a
helper macro in the `picotcp_socket_sync_client_interfaces` optional module
which simplifies this process.

```c
picotcp_socket_sync_client_connections(client, client_name, server, server_name)
```

This component also needs to be connected to a TimeServer component through the
`Timer` CAmkES RPC interface exposed in this CPP definition.

```c
picotcp_base_interfaces(pico_base)
```

There is a helper macro which simplifies this process that does not require the
optional module.

```c
picotcp_base_connections(instance, name, timeserver_interface)
```

Aside from the connections, there are some configuration options that need to
be set. There is a helper macro that simplifies this that does not require the
optional module.

```c
picotcp_base_configuration(instance, name, ip_addr, multicast_addr)
```

The macro configures the following attributes.

```c
attribute string pico_base_ip_addr = "";
attribute string pico_base_multicast_addr;
```

## Supported platforms

Currently, this component supports all platforms but requires the Ethdriver
component to be present in order to send data out through a Ethernet
connection.

## Limitations

Requests are synchronous, meaning that requests will block the client until it
finishes. Additionally, data transfer also require copying to and from a shared
buffer. This may present performance issues for applications that require high
network throughput. There are plans to improve this in the near future, and is
currently on a higher priority.
