<!--
     Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)

     SPDX-License-Identifier: CC-BY-SA-4.0
-->

# Ethdriver

This is a a CAmkES component that is intended to abstract over a platform's
Ethernet device and multiplex client access to it.

## Dependencies

This component needs to be used with the `single-threaded` module that performs
the initialisation process, and client request routing the component relies on.
The `picotcp-ethernet-async` module is an optional dependency which provides
helper CAmkES macros and an asynchronous data transfer interface for clients.

## Usage

For clients to use the functionality exposed by the component, the clients will have to connect to this CAmkES interface using the `seL4Ethdriver` connector.

```c
provides Ethdriver client;
```

There are also some additional configuration options for the DMA that need to
be set. To set this, change the following to be a page aligned number, and
replace `name` with the name of the Ethdriver instance.

```c
name.dma_pool = <desired DMA pool size>;
```

To use the asynchronous data transfer interface, the `picotcp-ethernet-async`
optional module provides several macros to add these extra interfaces, connect
the client to the component, and configure the necessary options as well. These
are:

```c
picotcp_ethernet_async_server_interfaces(name)

picotcp_ethernet_async_connections(name, client, driver)

picotcp_ethernet_async_configurations(name, client, driver)
```

## Supported platforms

Currently, this component supports the following platforms:
  - i.MX6
  - i.MX8MQ Evaluation Kit
  - TX2
  - Zynq7000
  - pc99, specifically the Intel 82574 and 82580 Ethernet controllers

Additional platforms require a corresponding clock system driver to be
implemented in the libplatsupport library of the
[`util_libs`](https://github.com/seL4/util_libs) repository, and the following
CPP defintions to be created in the `include` folder of this component (if the
platform is ARM-based):
  - `HARDWARE_ETHERNET_INTERFACES`
  - `HARDWARE_ETHERNET_INTERFACES`
  - `HARDWARE_ETHERNET_ATTRIBUTES`
  - `HARDWARE_ETHERNET_COMPOSITION`
  - `HARDWARE_ETHERNET_CONFIG`
  - and possibly, `HARDWARE_ETHERNET_EXTRA_IMPORTS`

## Limitations

Network performance may be an issue for network-demanding applications that
require throughputs close to Gigabit. This is because the Ethdriver by default
runs in its own address space and data transfer is done via copying in and out
to and from shared memory channels between the Ethdriver and the clients.
