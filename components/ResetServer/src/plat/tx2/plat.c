/*
 * Copyright 2020, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */

#include <bpmpserver_client.h>
#include <camkes.h>
#include <platsupport/interface_registration.h>
#include <utils/util.h>

#include "../../plat.h"

/* Prototypes for the functions and variables in templates that do not have
 * prototypes auto-generated yet */
extern void *bpmp_buf;
size_t bpmp_get_size(void);

static struct tx2_bpmp bpmp;

int plat_init(ps_io_ops_t *io_ops)
{
    if (!io_ops) {
        ZF_LOGE("io_ops is NULL");
        return -1;
    }

    int error = bpmpserver_interface_init(bpmp_buf, bpmp_get_size(), bpmp_call, &bpmp);
    if (error) {
        ZF_LOGE("Failed to initialise the BPMP server client interface");
        return -1;
    }

    /* Register this BPMP interface so that the reset driver can access it */
    error = ps_interface_register(&io_ops->interface_registration_ops, TX2_BPMP_INTERFACE,
                                  &bpmp, NULL);
    if (error) {
        ZF_LOGE("Failed to register the BPMP interface");
        return -1;
    }

    return 0;
}
