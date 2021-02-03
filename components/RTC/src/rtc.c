/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <camkes.h>
#include <platsupport/plat/rtc.h>
#include <camkes/io.h>
#include <utils/util.h>

rtc_time_date_t rtc_time_date(void)
{
    rtc_time_date_t time_date;
    int error UNUSED;
    ps_io_port_ops_t ops = {0};
    error = camkes_io_port_ops(&ops);
    assert(!error);
    error = rtc_get_time_date_reg(&ops, 0, &time_date);
    assert(!error);
    return time_date;
}

void pre_init(void)
{
    set_putchar(putchar_putchar);
}
