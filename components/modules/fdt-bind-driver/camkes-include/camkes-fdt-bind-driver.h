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

#define fdt_bind_drivers_interfaces() \
    emits Dummy init_1;       \
    consumes Dummy init_2;


#define fdt_bind_driver_connections() \
        connection seL4InitHardware bind_driver_conn(from init_1, to init_2);


#define fdt_bind_driver_configuration(path) \
    init_2.paths = path;
