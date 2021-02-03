/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <errno.h>

#include <platsupport/io.h>
#include <utils/util.h>
#include <clockserver_client.h>

typedef struct clockserver_interface_data {
    ps_io_ops_t *io_ops;
    int (*clockserver_init_clock)(clk_id_t id);
    int (*clockserver_set_gate_mode)(clock_gate_t gate, clock_gate_mode_t mode);
    freq_t (*clockserver_get_freq)(clk_id_t clk_id);
    freq_t (*clockserver_set_freq)(clk_id_t clk_id, freq_t hz);
    int (*clockserver_register_child)(clk_id_t parent, clk_id_t child);
} clockserver_interface_data_t;

static clockserver_interface_data_t clockserver_private_data;

/* Note: No clk_t->init or clk_t->recal as these are handled by the server,
 * also no register child for now as there are limitations */

static freq_t clockserver_get_freq_wrapper(clk_t *clk)
{
    clock_sys_t *clock_sys = clk->clk_sys;
    clockserver_interface_data_t *clockserver_data = clock_sys->priv;

    return clockserver_data->clockserver_get_freq(clk->id);
}

static freq_t clockserver_set_freq_wrapper(clk_t *clk, freq_t hz)
{
    clock_sys_t *clock_sys = clk->clk_sys;
    clockserver_interface_data_t *clockserver_data = clock_sys->priv;

    freq_t set_freq = clockserver_data->clockserver_set_freq(clk->id, hz);

    /* Set the requested frequency */
    clk->req_freq = hz;

    return set_freq;
}

static clk_t *clockserver_get_clock_wrapper(clock_sys_t *clock_sys, enum clk_id id)
{
    clockserver_interface_data_t *clockserver_data = clock_sys->priv;

    int error = clockserver_data->clockserver_init_clock(id);
    if (error) {
        return NULL;
    }

    /* Allocate a clk_t structure and fill it in */
    clk_t *ret_clk = NULL;
    error = ps_calloc(&clockserver_data->io_ops->malloc_ops, 1, sizeof(clk_t), (void **) &ret_clk);
    if (error) {
        return NULL;
    }

    ret_clk->id = id;
    /* 'priv' needs to be setup for the function wrappers to not throw EINVAL,
     * we actually use 'priv' */
    ret_clk->priv = (void *)((uintptr_t) 0xDEADBEEF);
    ret_clk->clk_sys = clock_sys;
    ret_clk->get_freq = clockserver_get_freq_wrapper;
    ret_clk->set_freq = clockserver_set_freq_wrapper;

    return ret_clk;
}

static int clockserver_gate_enable_wrapper(clock_sys_t *clock_sys, enum clock_gate gate,
                                           enum clock_gate_mode mode)
{
    clockserver_interface_data_t *clockserver_data = clock_sys->priv;

    return clockserver_data->clockserver_set_gate_mode(gate, mode);
}

int clockserver_interface_init(ps_io_ops_t *io_ops,
                               int (*clockserver_init_clock)(clk_id_t id),
                               int (*clockserver_set_gate_mode)(clock_gate_t gate, clock_gate_mode_t mode),
                               freq_t (*clockserver_get_freq)(clk_id_t clk_id),
                               freq_t (*clockserver_set_freq)(clk_id_t clk_id, freq_t hz),
                               int (*clockserver_register_child)(clk_id_t parent, clk_id_t child),
                               clock_sys_t *clock_sys)
{
    if (!io_ops) {
        ZF_LOGE("io_ops is NULL");
        return -EINVAL;
    }

    if (!clockserver_init_clock) {
        ZF_LOGE("clockserver_init_clock is NULL");
        return -EINVAL;
    }

    if (!clockserver_set_gate_mode) {
        ZF_LOGE("clockserver_set_gate_mode is NULL");
        return -EINVAL;
    }

    if (!clockserver_get_freq) {
        ZF_LOGE("clockserver_get_freq is NULL");
        return -EINVAL;
    }

    if (!clockserver_set_freq) {
        ZF_LOGE("clockserver_set_freq is NULL");
        return -EINVAL;
    }

    if (!clockserver_register_child) {
        ZF_LOGE("clockserver_register_child is NULL");
        return -EINVAL;
    }

    if (!clock_sys) {
        ZF_LOGE("provided clock_sys_t is NULL");
        return -EINVAL;
    }

    clockserver_private_data.io_ops = io_ops;
    clockserver_private_data.clockserver_init_clock = clockserver_init_clock;
    clockserver_private_data.clockserver_set_gate_mode = clockserver_set_gate_mode;
    clockserver_private_data.clockserver_get_freq = clockserver_get_freq;
    clockserver_private_data.clockserver_set_freq = clockserver_set_freq;
    clockserver_private_data.clockserver_register_child = clockserver_register_child;

    clock_sys->priv = &clockserver_private_data;
    clock_sys->get_clock = clockserver_get_clock_wrapper;
    clock_sys->gate_enable = clockserver_gate_enable_wrapper;

    return 0;
}
