<!--
     Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)

     SPDX-License-Identifier: CC-BY-SA-4.0
-->
# fdt-bind-driver

This is a CAmkES driver module that specifies a list of devices to intialize
drivers for.

Each driver needs to have been declared with `PS_DRIVER_MODULE_DEFINE` that is
defined in `libplatsupport`.  During initialization of the component, the device
tree will be parsed and if a driver has been defined with a valid compatibility
string then an attempt will be made to intialize the driver.

## Dependencies

This module needs to be used with the `single-threaded` module that performs
the intialization process this module relies on.

## Camkes module API

The following CPP definitions create the required interfaces and connection instances
that implement this module for a component. This module can only be added once to
each component. Multiple device paths can be given to the driver to select multiple
devices.

```c
/**
 * @brief      Declare component interfaces and default list of devices
 *
 * @param      default_bind_paths  The default bind paths
 */
fdt_bind_drivers_interfaces(string default_bind_paths[]);

/**
 * Creates internal component connections
 */
fdt_bind_driver_connections();

/**
 * @brief      Configure the module
 *
 * @param      instance  The name of the component instance.
 * @param      devices   A list of paths in the device tree describing the
 *                       system.
 */
fdt_bind_driver_configuration_override(string instance, string devices[]);
```


## Usage

The following shows a driver's partial intialization function's implementation and
how it is registered as a possible intialization function for devices with valid
compatibility strings.

```c

#include <platsupport/driver_module.h>

static int uart_pl011_init(ps_io_ops_t *io_ops, const char *device_path)
{
    // ...

    return 0;
}

static const char*compatible_strings[] = {
    "arm,pl011",
    NULL
};


PS_DRIVER_MODULE_DEFINE(uart_pl011, compatible_strings, uart_pl011_init);

```

A CMake configuration is required to force the driver to be linked into the final component binary.

```cmake
# Force the driver module definition to be linked into final binary
target_link_libraries(driver_library "-Wl,--undefined=uart_pl011_ptr")
```

The camkes component definition can provide a default list of devices to bind to.
This can then be overridden on a per instance basis.

```c

#include <camkes-fdt-bind-driver.h>

component Driver {
    // ..
    fdt_bind_drivers_interfaces(["/soc/uart@f7112000"]);

    composition {
        // ...
        fdt_bind_driver_connections();
    }

}


assembly {
    composition {
        component Driver uarta;
    }
    configuration {
        fdt_bind_driver_configuration_override(uarta, ["/soc/uart@f8015000"]);
    }
}

```
