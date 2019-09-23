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

#include <assert.h>
#include <stdio.h>
#include <camkes.h>
#include <camkes/io.h>
#include <platsupport/io.h>
#include <platsupport/gpio.h>
#include <utils/util.h>

#include "gpio.h"
#include "mux.h"

static ps_io_ops_t io_ops;

void post_init(void)
{
    int error = big_lock_lock();
    ZF_LOGF_IF(error, "Failed to lock GPIOMUX server");

    error = camkes_io_ops(&io_ops);
    ZF_LOGF_IF(error, "Failed to get camkes_io_ops");

    /* initialise the MUX first, on some platforms, the GPIO controller relies on the MUX */
    error = mux_component_init(&io_ops);
    ZF_LOGF_IF(error, "Failed to initialise the MUX component of the server");

    error = gpio_component_init(&io_ops);
    ZF_LOGF_IF(error, "Failed to initialise the GPIO component of the server");

    error = big_lock_unlock();
    ZF_LOGF_IF(error, "Failed to unlock GPIOMUX server");
}
