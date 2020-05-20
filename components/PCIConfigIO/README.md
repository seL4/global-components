<!--
     Copyright 2020, Data61
     Commonwealth Scientific and Industrial Research Organisation (CSIRO)
     ABN 41 687 119 230.

     This software may be distributed and modified according to the terms of
     the BSD 2-Clause license. Note that NO WARRANTY is provided.
     See "LICENSE_BSD2.txt" for details.

     @TAG(DATA61_BSD)
-->

# PCIConfigIO 

This is a CAmkES component that is inteded to abstract over the PCI
configuration process and multiplex client requests.

## Dependencies

There are currently no dependencies for this component.

## Usage

For clients to use the functionality exposed by the component, the clients will
need to be connected to this CAmkES interface using the `seL4RPCCall`
connector.

```c
provides PCIConfig pci_config;
```

This component will also need to be connected to SerialServer instance through
this interface.

```c
uses PutChar putchar;
```

## Supported platforms

This component only supports the pc99 platform.

