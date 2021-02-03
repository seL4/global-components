/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#pragma once

#include <platsupport/ltimer.h>
#include <platsupport/io.h>
#include <utils/util.h>

void plat_pre_init(void) WEAK;
void plat_post_init(ltimer_t *ltimer) WEAK;
