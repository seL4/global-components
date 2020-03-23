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

#include <assert.h>
#include <string.h>
#include <errno.h>

#include <camkes.h>
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
void *the_bpmp_buf(seL4_Word);
int the_bpmp_largest_badge(void);

static struct tx2_bpmp bpmp;
static ps_io_ops_t io_ops;
/* MSG_MIN_SZ is from tx2bpmp/bpmp.h */
static char bpmp_rx_buf[MSG_MIN_SZ] = {0};

int the_bpmp_call(int mrq, size_t tx_size, size_t *bytes_rxd)
{
    if (!bytes_rxd) {
        return -EINVAL;
    }

    int error = bpmp_server_lock();
    ZF_LOGF_IF(error, "Failed to lock bpmp server");

    seL4_Word client_id = the_bpmp_get_sender_id();
    void *client_buf = the_bpmp_buf(client_id);
    assert(client_buf);

    int ret = tx2_bpmp_call(&bpmp, mrq, client_buf, tx_size, &bpmp_rx_buf, sizeof(bpmp_rx_buf));
    if (ret >= 0) {
        /* Success! Copy the contents of the rx buf into the shared memory region */
        memcpy(client_buf, bpmp_rx_buf, ret);
        /* Zero out the contents of the rx buf for the next request */
        memset(bpmp_rx_buf, 0, sizeof(bpmp_rx_buf));
        /* Write the amount of bytes received from the BPMP module */
        *bytes_rxd = ret;
        /* Return zero as a success code */
        ret = 0;
    }

    error = bpmp_server_unlock();
    ZF_LOGF_IF(error, "Failed to unlock BPMP server");

    return ret;
}

void pre_init(void)
{
    int error = bpmp_server_lock();
    ZF_LOGF_IF(error, "Failed to lock bpmp server");

    error = camkes_io_ops(&io_ops);
    ZF_LOGF_IF(error, "Failed to get IO ops");

    error = tx2_bpmp_init(&io_ops, &bpmp);
    ZF_LOGF_IF(error, "Failed to initialise the BPMP structure");

    error = bpmp_server_unlock();
    ZF_LOGF_IF(error, "Failed to unlock bpmp server");
}
