/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <platsupport/reset.h>

int resetserver_interface_init(int (*resetserver_assert)(reset_id_t id),
                               int (*resetserver_deassert)(reset_id_t id),
                               reset_sys_t *reset_sys);
