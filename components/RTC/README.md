<!--
     Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)

     SPDX-License-Identifier: CC-BY-SA-4.0
-->

# RTC

This is a CAmkES component that is inteded to abstract over a platform's Real
Time Clock (RTC) and multiplex client access to it.

## Dependencies

There are currently no dependencies for this component.

## Usage

For clients to use the functionality exposed by the component, the clients will
have to connect to following CAmkES interface with the `seL4RPCCall` connector.

```c
provides RTC rtc;
```

## Supported platforms

Currently, this component supports the following platforms:
  - pc99

## Limitations

There are currently no limitations with this component.
