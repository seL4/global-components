<!--
     Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)

     SPDX-License-Identifier: CC-BY-SA-4.0
-->

# FileServer

This is a CAmkES component that is intended to multiplex client accesses to a
CPIO archive.

## Dependencies

There are currently no dependencies for this component.

## Usage

For clients to use the functionality exposed by the component, the clients will
need to be connected to this CAmkES interface using the `seL4RPCDataport`
connector.

```
provides FileServerInterface fs_ctrl;
```

## Supported platforms

This component is supported on all platforms.

## Limitations

This component is currently only able to serve files from a memory-backed CPIO
archive. Thus, the amount of files that it can serve is limited to the size of
memory.

