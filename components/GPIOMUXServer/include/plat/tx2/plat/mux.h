/*
 * Copyright 2019, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */
#pragma once

#define HARDWARE_MUX_INTERFACES     \
    emits Dummy dummy_mux_source;   \
    consumes Dummy mux;

#define HARDWARE_MUX_ATTRIBUTES

#define HARDWARE_MUX_COMPOSITION                                        \
    connection seL4DTBHardware mux_conn(from dummy_mux_source, to mux);

#define HARDWARE_MUX_CONFIG                 \
    mux.dtb = dtb({"path" : "/pinmux@2430000"});
