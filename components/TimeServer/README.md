<!--
     Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)

     SPDX-License-Identifier: CC-BY-SA-4.0
-->

# TimeServer

This is a CAmkES component that is intended to abstract over a platform's timer
and multiplex client access to it.

## Dependencies

There are currently no dependencies for this component.

## Usage

For clients to use the functionality exposed by the component, the clients will
need to be connected to this CAmkES interface using the `seL4TimeServer`
connector.

```c
provides Timer the_timer;
```

The component also exposes a CAmkES emits interface which can be connected to
clients with the `seL4GlobalAsynchCallback` connector to allow a callback to be
automatically called on the client's side when the component signals a timeout
has occurred.

```c
emits Notification timer_notification;
```

The `timers_per_client` attribute can also be set to a maximum limit of active
timers for all clients.

## Supported platforms

Currently, this component supports the following platforms:
  - i.MX6
  - i.MX8
  - Odroid C2
  - Exynos5-based platforms (Odroid XU, Odroid XU4)
  - Beaglebone Black (possibly Beaglebone Blue as well)
  - pc99
  - QEMU ARM Virt configuration
  - TK1
  - TX1
  - TX2
  - Zynq7000
  - ZynqMP

Additional platforms require a corresponding timer driver to be
implemented in the libplatsupport library of the
[`util_libs`](https://github.com/seL4/util_libs) repository, and the following
CPP defintions to be created in the `include` folder of this component:
  - `HARDWARE_TIMER_COMPONENT`
  - `HARDWARE_TIMER_INTERFACES`
  - `HARDWARE_TIMER_ATTRIBUTES`
  - `HARDWARE_TIMER_COMPOSITION`
  - `HARDWARE_TIMER_CONFIG`

## Limitations

Right now, some of the timer drivers assume that the clocks for the timers are
initialised (at least for ARM and possibly RISC V platforms). This is mainly a
side-effect of boards being booted by U-Boot which handles the clock
initialisation.
