/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#define fdt_bind_drivers_interfaces(default_bind_paths) \
    emits Dummy fdt_bind_drivers_1;       \
    consumes Dummy fdt_bind_drivers_2; \
    attribute string fdt_bind_driver_paths[] = default_bind_paths


#define fdt_bind_driver_connections() \
    connection seL4InitHardware bind_driver_conn(from fdt_bind_drivers_1, to fdt_bind_drivers_2)


#define fdt_bind_driver_configuration_override(instance, bind_paths) \
    instance.fdt_bind_driver_paths = bind_paths
