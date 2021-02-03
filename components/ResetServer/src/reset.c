/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <autoconf.h>

#include <assert.h>
#include <stdbool.h>
#include <sel4/sel4.h>
#include <camkes/io.h>
#include <platsupport/reset.h>
#include <utils/util.h>

/*
 * Gotchas:
 *  - there is no access control at the moment, this means that clients can
 *    perform a denial-of-service attack on other clients by asserting the reset
 *    line on devices
 */

/* reset_sys for interacting with the reset lines */
static reset_sys_t reset_sys;
/* io_ops structure to access IO resources with */

int the_reset_assert_reset(reset_id_t id)
{
    return reset_sys_assert(&reset_sys, id);
}

int the_reset_deassert_reset(reset_id_t id)
{
    return reset_sys_deassert(&reset_sys, id);
}

int ResetServer_init(ps_io_ops_t *io_ops)
{
    int error;

    error = reset_sys_init(io_ops, NULL, &reset_sys);
    ZF_LOGF_IF(error, "Failed to initialise the reset subsystem with the BPMP");
    return 0;
}
