/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <errno.h>

#include <utils/util.h>
#include <gpiomuxserver.h>

typedef struct gpiomuxserver_gpio_interface_data {
    int (*gpiomuxserver_init_pin)(gpio_id_t pin_id, gpio_dir_t dir);
    int (*gpiomuxserver_set_level)(gpio_id_t pin_id, gpio_level_t level);
    int (*gpiomuxserver_read_level)(gpio_id_t pin_id);
} gpiomuxserver_gpio_interface_data_t;

static gpiomuxserver_gpio_interface_data_t gpiomuxserver_gpio_private_data;

static int gpiomuxserver_init_wrapper(gpio_sys_t *gpio_sys, gpio_id_t id, enum gpio_dir dir, gpio_t *gpio)
{
    gpiomuxserver_gpio_interface_data_t *gpiomuxserver_data = gpio_sys->priv;

    int error = gpiomuxserver_data->gpiomuxserver_init_pin(id, dir);
    if (error) {
        return error;
    }

    /* Fill out GPIO struct and return it */
    gpio->id = id;
    gpio->gpio_sys = gpio_sys;

    return 0;
}

static int gpiomuxserver_set_level_wrapper(gpio_t *gpio, enum gpio_level level)
{
    gpiomuxserver_gpio_interface_data_t *gpiomuxserver_data = gpio->gpio_sys->priv;

    return gpiomuxserver_data->gpiomuxserver_set_level(gpio->id, level);
}

static int gpiomuxserver_read_level_wrapper(gpio_t *gpio)
{
    gpiomuxserver_gpio_interface_data_t *gpiomuxserver_data = gpio->gpio_sys->priv;

    return gpiomuxserver_data->gpiomuxserver_read_level(gpio->id);
}

int gpiomuxserver_gpio_interface_init(int (*gpiomuxserver_init_pin)(gpio_id_t pin_id, gpio_dir_t dir),
                                      int (*gpiomuxserver_set_level)(gpio_id_t pin_id, gpio_level_t level),
                                      int (*gpiomuxserver_read_level)(gpio_id_t pin_id),
                                      gpio_sys_t *gpio_sys)
{
    if (!gpiomuxserver_init_pin) {
        ZF_LOGE("gpiomuxserver_init_pin is NULL");
        return -EINVAL;
    }

    if (!gpiomuxserver_set_level) {
        ZF_LOGE("gpiomuxserver_set_level is NULL");
        return -EINVAL;
    }

    if (!gpiomuxserver_read_level) {
        ZF_LOGE("gpiomuxserver_read_level is NULL");
        return -EINVAL;
    }

    if (!gpio_sys) {
        ZF_LOGE("gpio_sys is NULL");
        return -EINVAL;
    }

    gpiomuxserver_gpio_private_data.gpiomuxserver_init_pin = gpiomuxserver_init_pin;
    gpiomuxserver_gpio_private_data.gpiomuxserver_set_level = gpiomuxserver_set_level;
    gpiomuxserver_gpio_private_data.gpiomuxserver_read_level = gpiomuxserver_read_level;

    gpio_sys->priv = &gpiomuxserver_gpio_private_data;
    gpio_sys->init = gpiomuxserver_init_wrapper;
    gpio_sys->set_level = gpiomuxserver_set_level_wrapper;
    gpio_sys->read_level = gpiomuxserver_read_level_wrapper;

    return 0;
}
