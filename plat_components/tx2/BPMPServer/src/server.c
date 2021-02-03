/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <assert.h>
#include <string.h>
#include <errno.h>

#include <camkes/io.h>
#include <tx2bpmp/bpmp.h>
#include <utils/util.h>

/*
 * Gotchas:
 *  - There is no access control at the moment for the BPMP, so two components
 *    can interfere with each other by asking the BPMP to perform actions that
 *    interfere with one another. E.g. One component turning on a clock and the
 *    other component turning off that clock.
 */

/* Prototypes for the functions in templates that do not have prototypes
 * auto-generated yet */
seL4_Word the_bpmp_get_sender_id(void);
seL4_Word the_bpmp_enumerate_badge(unsigned int i);
void *the_bpmp_buf(seL4_Word);

struct tx2_bpmp bp;
static struct tx2_bpmp *bpmp;
/* MSG_MIN_SZ is from tx2bpmp/bpmp.h */
static char bpmp_rx_buf[MSG_MIN_SZ] = {0};

int the_bpmp_call(int mrq, size_t tx_size, size_t *bytes_rxd)
{
    if (!bytes_rxd) {
        return -EINVAL;
    }

    seL4_Word client_id = the_bpmp_get_sender_id();
    void *client_buf = the_bpmp_buf(the_bpmp_enumerate_badge(client_id));
    assert(client_buf);

    int ret = tx2_bpmp_call(bpmp, mrq, client_buf, tx_size, &bpmp_rx_buf, sizeof(bpmp_rx_buf));
    if (ret >= 0) {
        /* Success! Copy the contents of the rx buf into the shared memory region */
        memcpy(client_buf, bpmp_rx_buf, ret);
        /* Zero out the contents of the rx buf for the next request */
        // memset(bpmp_rx_buf, 0, sizeof(bpmp_rx_buf));
        /* Write the amount of bytes received from the BPMP module */
        *bytes_rxd = ret;
        /* Return zero as a success code */
        ret = 0;
    }

    return ret;
}
static int interface_search_handler(void *handler_data, void *interface_instance, char **properties)
{
    /* Select the first one that is registered */
    bpmp = (struct tx2_bpmp *) interface_instance;
    return PS_INTERFACE_FOUND_MATCH;
}


int bpmp_server_init(ps_io_ops_t *io_ops)
{

    int error = ps_interface_find(&io_ops->interface_registration_ops, TX2_BPMP_INTERFACE,
                                  interface_search_handler, NULL);
    ZF_LOGF_IF(error, "Failed to find bpmp driver");
    printf("found interface\n");


    return 0;
}
