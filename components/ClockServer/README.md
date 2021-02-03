<!--
     Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)

     SPDX-License-Identifier: CC-BY-SA-4.0
-->

# ClockServer

This is a CAmkES component that is intended to abstract over a platform's clock
system and multiplex client access to it.

## Dependencies

This component needs to be used with the `single-threaded` module that performs
the initialisation process, and client request routing the component relies on.

## Usage

For clients to use the functionality exposed by the component, the clients will
have to connect to the CAmkES interfaces that the following CPP defintion
expose.

```c
ClockServer_server_interfaces(the_clock)
```

The definition exposes a `Clock` RPC interface that is expected to be connected
via the `seL4RPCNoThreads` connector. However, there is a helper macro which
simplifies this process and also registers a clock interface on the client's
interface registration service (`ps_interface_ops_t`).

```c
ClockServer_client_connections(client_name, client, server_name, server)
```

## Supported platforms

Currently, this component supports the following platforms:
  - Exynos5-based platforms (Odroid XU, Odroid XU4)
  - i.MX6
  - TX2

Additional platforms require a corresponding clock system driver to be
implemented in the libplatsupport library of the
[`util_libs`](https://github.com/seL4/util_libs) repository, and the following
CPP defintions to be created in the `include` folder of this component:
  - `HARDWARE_CLOCK_COMPONENT`
  - `HARDWARE_CLOCK_INTERFACES`
  - `HARDWARE_CLOCK_ATTRIBUTES`
  - `HARDWARE_CLOCK_COMPOSITION`
  - `HARDWARE_CLOCK_CONFIG`
  - and possibly, `HARDWARE_CLOCK_EXTRA_INTERFACES`, and
    `HARDWARE_CLOCK_EXTRA_IMPORTS`

## Limitations

There is a basic form of access control for the clocks but not for the clock
gates. This means that clients can interfere with each other by disabling specific
clock gates which stop clock signals from a particular clock. There are plans
to implement a form of access control for the clock gates.

