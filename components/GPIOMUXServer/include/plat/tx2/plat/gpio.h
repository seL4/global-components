/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#pragma once

#define HARDWARE_GPIO_INTERFACES        \
    emits Dummy dummy_gpio_source;      \
    consumes Dummy gpio;

#define HARDWARE_GPIO_ATTRIBUTES

#define HARDWARE_GPIO_COMPOSITION                                       \
    connection seL4DTBHardware gpio_conn(from dummy_gpio_source, to gpio);

#define HARDWARE_GPIO_CONFIG            \
    gpio.dtb = dtb({"path" : "/gpio@2200000"});
