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

#define HARDWARE_GPIO_INTERFACES        \
    emits Dummy dummy_gpio_source;      \
    consumes Dummy gpio;

#define HARDWARE_GPIO_ATTRIBUTES

#define HARDWARE_GPIO_COMPOSITION                                       \
    connection seL4DTBHardware gpio_conn(from dummy_gpio_source, to gpio);

#define HARDWARE_GPIO_CONFIG            \
    gpio.dtb = dtb({"path" : "/gpio@2200000"});
