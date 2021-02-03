/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <assert.h>
#include <stdio.h>
#include <camkes/io.h>
#include <platsupport/io.h>
#include <utils/util.h>

#include "mux.h"

/* Reference to the shared IO ops structure */
static ps_io_ops_t *io_ops_ref;

int mux_component_plat_init(ps_io_ops_t *io_ops) WEAK;

int the_mux_feature_enable(mux_feature_t mux_feature, mux_gpio_dir_t mux_gpio_dir)
{
    return mux_feature_enable(&io_ops_ref->mux_sys, mux_feature, mux_gpio_dir);
}

int the_mux_feature_disable(mux_feature_t mux_feature)
{
    return mux_feature_disable(&io_ops_ref->mux_sys, mux_feature);
}

int mux_component_init(ps_io_ops_t *io_ops)
{
    int error = 0;

    /* save a copy of the io_ops structure */
    io_ops_ref = io_ops;

    if (mux_component_plat_init) {
        error = mux_component_plat_init(io_ops);
    } else {
        error = mux_sys_init(io_ops, NULL, &io_ops->mux_sys);
    }

    if (error) {
        ZF_LOGE("Failed to initialise the MUX subsystem");
    }

    return error;
}
