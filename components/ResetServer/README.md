<!--
     Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)

     SPDX-License-Identifier: CC-BY-SA-4.0
-->

# ResetServer

This is a CAmkES component that is intended to abstract over a platform's reset
controller and multiplex client access to it.

## Dependencies

This component needs to be used with the `single-threaded` module that performs
the initialisation process, and client request routing the component relies on.

## Usage

For clients to use the functionality exposed by the component, the clients will
have to connect to the CAmkES interfaces that the following CPP definition
expose.

```c
ResetServer_server_interfaces(the_reset)
```

The definition exposed a `Reset` RPC interface that is expected to be connected
via the `seL4RPCNoThreads` connector. However, there is a helper macro which
simplifies this process and also registers a clock interface on the client's
interface registration service (`ps_interface_ops_t`).

```c
ResetServer_client_connections(client_name, client, server_name, server)
```

## Supported platforms

Currently, this component supports the following platforms:
  - TX2

Additional platforms require a corresponding reset controller driver to be
implemented in the libplatsupport library of the
[`util_libs`](https://github.com/seL4/util_libs) repository, and the following
CPP defintions to be created in the `include` folder of this component:
  - `HARDWARE_RESET_COMPONENT`
  - `HARDWARE_RESET_INTERFACES`
  - `HARDWARE_RESET_ATTRIBUTES`
  - `HARDWARE_RESET_COMPOSITION`
  - `HARDWARE_RESET_CONFIG`
  - and possibly, `HARDWARE_RESET_EXTRA_INTERFACES`, and
    `HARDWARE_RESET_EXTRA_IMPORTS`

## Limitations

There is no form of access control for the reset lines, this means that clients
can interfere with each other by asserting of specific reset lines causing
certain devices on a platform to be locked up.
