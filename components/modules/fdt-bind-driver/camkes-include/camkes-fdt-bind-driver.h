/*
 * Copyright 2020, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */

#define fdt_bind_drivers_interfaces(default_bind_paths) \
    emits Dummy fdt_bind_drivers_1;       \
    consumes Dummy fdt_bind_drivers_2; \
    attribute string fdt_bind_driver_paths[] = default_bind_paths


#define fdt_bind_driver_connections() \
    connection seL4InitHardware bind_driver_conn(from fdt_bind_drivers_1, to fdt_bind_drivers_2)


#define fdt_bind_driver_configuration_override(instance, bind_paths) \
    instance.fdt_bind_driver_paths = bind_paths
