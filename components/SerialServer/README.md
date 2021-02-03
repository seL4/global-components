<!--
     Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)

     SPDX-License-Identifier: CC-BY-SA-4.0
-->

# SerialServer

This is a CAmkES component that is intended to abstract over a platform's
serial device and multiplex client access to it.

## Dependencies

This component currently has no dependencies.

## Usage

For clients to use the functionality exposed by the component, the clients will
need to be connected to these CAmkES interfaces using the `seL4RPCCall` and
`seL4SerialServer` connector respectively.

```c
provides PutChar processed_putchar *or* provides PutChar raw_putchar;
provides GetChar getchar;
```

Additionally, this other interface needs to be connected to a TimeServer
component instance.

```c
uses Timer timeout;
```

This component is capable of an asynchronous data-transfer interface with the
use of virtqueues, and this requires these interfaces to be connected using the
`seL4VirtQueues` and `seL4GlobalAsynchCallback` connector respectively.

```c
maybe uses VirtQueueDev read; *and* maybe uses VirtQueueDev write;
emits Callback self_read; *to* maybe consumes Callback serial_read_wait; *and* emits Callback self_write; *to* maybe consumes Callback serial_write_wait;
```

An example of this asynchronous interface in action can be found in the
`serialserver_loopback` application in the [camkes
respository](https://github.com/seL4/camkes/blob/master/apps/serialserver_loopback/serialserver_loopback.camkes).

## Supported platforms

Currently, this component supports the following platforms:
  - ARM-based platforms that have an existing serial driver implementation in libplatsupport
  - pc99

Additional platforms simply only require a corresponding serial driver to be
implemented in the libplatsupport library of the
[`util_libs`](https://github.com/seL4/util_libs) respository.
