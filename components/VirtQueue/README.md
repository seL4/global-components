<!--
     Copyright 2020, Data61
     Commonwealth Scientific and Industrial Research Organisation (CSIRO)
     ABN 41 687 119 230.

     This software may be distributed and modified according to the terms of
     the BSD 2-Clause license. Note that NO WARRANTY is provided.
     See "LICENSE_BSD2.txt" for details.

     @TAG(DATA61_BSD)
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
