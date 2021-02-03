/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include <camkes.h>
#include <camkes/io.h>
#include <camkes/dma.h>
#include <ethdrivers/raw.h>
#include <ethdrivers/zynq7000.h>
#include <platsupport/io.h>
#include <platsupport/clock.h>
#include <platsupport/irq.h>



static int init_device(ps_io_ops_t *io_ops)
{

    // This may reinitialise the clocks, timeserver might not like this,
    // but zynq7000/uboot/zynq_gem.c requires access to the SLCR registers
    // to configure some clocks related to the Ethernet device.
    return clock_sys_init(io_ops, &io_ops->clock_sys);
}


CAMKES_PRE_INIT_MODULE_DEFINE(ethdriver_setup, init_device);

