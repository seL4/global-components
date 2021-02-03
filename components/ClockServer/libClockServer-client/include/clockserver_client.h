/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <platsupport/clock.h>

int clockserver_interface_init(ps_io_ops_t *io_ops,
                               int (*clockserver_init_clock)(clk_id_t id),
                               int (*clockserver_set_gate_mode)(clock_gate_t gate, clock_gate_mode_t mode),
                               freq_t (*clockserver_get_freq)(clk_id_t clk_id),
                               freq_t (*clockserver_set_freq)(clk_id_t clk_id, freq_t hz),
                               int (*clockserver_register_child)(clk_id_t parent, clk_id_t child),
                               clock_sys_t *clock_sys);
