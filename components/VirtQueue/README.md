<!--
     Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)

     SPDX-License-Identifier: CC-BY-SA-4.0
-->

# VirtQueue

This is a CAmkES component used to create `seL4VirtQueues` connections.

## Dependencies

There are currently no dependencies for this component.

## Usage

To initialise `seL4VirtQueues` connections, connect the 'from' side of the
connection to the CAmkES interface exposed by this component.

```c
provides VirtQueue init;
```

## Supported platforms

This component is not platform-specific and will support all platforms.

## Limitations

Although `seL4VirtQueues` connections should not require a separate dedicated
component to be created, for legacy reasons, this component is still necessary
to create seL4VirtQueue connections.
