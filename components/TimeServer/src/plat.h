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

#include <platsupport/ltimer.h>
#include <platsupport/io.h>
#include <utils/util.h>

void plat_pre_init(void) WEAK;
void plat_post_init(ltimer_t *ltimer) WEAK;
