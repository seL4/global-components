/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#pragma once

#define HARDWARE_MUX_INTERFACES     \
    emits Dummy dummy_source;       \
    consumes Dummy mux_dev_0;       \
    consumes Dummy mux_dev_1;       \
    consumes Dummy mux_dev_2;       \
    consumes Dummy mux_dev_3;       \
    consumes Dummy mux_dev_4;

#define HARDWARE_MUX_ATTRIBUTES

#define HARDWARE_MUX_COMPOSITION    \
    connection seL4DTBHardware mux_conn_0(from dummy_source, to mux_dev_0);     \
    connection seL4DTBHardware mux_conn_1(from dummy_source, to mux_dev_1);     \
    connection seL4DTBHardware mux_conn_2(from dummy_source, to mux_dev_2);     \
    connection seL4DTBHardware mux_conn_3(from dummy_source, to mux_dev_3);     \
    connection seL4DTBHardware mux_conn_4(from dummy_source, to mux_dev_4);

#define HARDWARE_MUX_CONFIG         \
    mux_dev_0.dtb = dtb({"path" : "/soc/pinctrl@13400000" });   \
    mux_dev_1.dtb = dtb({"path" : "/soc/pinctrl@13410000" });   \
    mux_dev_2.dtb = dtb({"path" : "/soc/pinctrl@14000000" });   \
    mux_dev_3.dtb = dtb({"path" : "/soc/pinctrl@14010000" });   \
    mux_dev_4.dtb = dtb({"path" : "/soc/pinctrl@3860000" });
