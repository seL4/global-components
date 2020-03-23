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

#include <platsupport/io.h>
#include <platsupport/gpio.h>

/* The header below abstracts over the platform-specific headers. Some
 * platforms have headers outside of libplatsupport and in other places. */
#include <gpiomuxserver_plat.h>

int gpiomuxserver_gpio_interface_init(int (*gpiomuxserver_init_pin)(gpio_id_t pin_id, gpio_dir_t dir),
                                      int (*gpiomuxserver_set_level)(gpio_id_t pin_id, gpio_level_t level),
                                      int (*gpiomuxserver_read_level)(gpio_id_t pin_id),
                                      gpio_sys_t *gpio_sys);

int gpiomuxserver_mux_interface_init(int (*gpiomuxserver_feature_enable)(mux_feature_t mux_feature,
                                                                         mux_gpio_dir_t mux_gpio_dir),
                                     int (*gpiomuxserver_feature_disable)(mux_feature_t mux_feature),
                                     mux_sys_t *mux_sys);
