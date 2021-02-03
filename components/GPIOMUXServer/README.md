<!--
     Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)

     SPDX-License-Identifier: CC-BY-SA-4.0
-->

# GPIOMUXServer

This is a CAmkES component that is intended to abstract over a platform's GPIO
and pin multiplexer controller, and multiplex client access to it.

## Dependencies

This component needs to be used with the `single-threaded` module that performs
the initialisation process, and client request routing the component relies on.

## Usage

For clients to use the functionality exposed by the component, the clients will
have to connect to the CAmkES interfaces that the following CPP defintions
expose.

```c
GPIOServer_server_interfaces(the_gpio)
MUXServer_server_interfaces(the_mux)
```

The definitions exposes a `GPIO` and `MUX` RPC interface that is expected to be
connected via the `seL4RPCNoThreads` connector. However, there are helper
macros which simplifies this process and also registers a GPIO and mux
interface on the client's interface registration service
(`ps_interface_ops_t`).

```c
ClockServer_client_connections(client_name, client, server_name, server)
MUXServer_client_connections(client_name, client, server_name, server)
```

## Supported platforms

Currently, this component supports the following platforms:
  - Exynos5-based platforms (Odroid XU, Odroid XU4)
  - TX2

Additional platforms require a corresponding GPIO and pin multiplexer
controller driver to be implemented in the libplatsupport library of the
[`util_libs`](https://github.com/seL4/util_libs) repository, and the following
CPP defintions to be created in the `include` folder of this component:
  - `HARDWARE_GPIO_COMPONENT`
  - `HARDWARE_GPIO_INTERFACES`
  - `HARDWARE_GPIO_ATTRIBUTES`
  - `HARDWARE_GPIO_COMPOSITION`
  - `HARDWARE_GPIO_CONFIG`
  - `HARDWARE_MUX_COMPONENT`
  - `HARDWARE_MUX_INTERFACES`
  - `HARDWARE_MUX_ATTRIBUTES`
  - `HARDWARE_MUX_COMPOSITION`
  - `HARDWARE_MUX_CONFIG`

## Limitations

The pin multiplexing part of the component may interfere with the GPIO part of
the component, that is, if a client requests that a feature be switched on via
the pin multiplexer, another client may lose access to a GPIO pin. There is no
form of access control with regards to both the GPIO pins and the pin
multiplexer features.
