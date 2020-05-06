/*
 * Copyright 2019, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(DATA61_GPL)
 */

#include <autoconf.h>

#include <string.h>
#include <camkes.h>
#include <pico_stack.h>
#include <sel4/sel4.h>
#include <utils/util.h>
#include <sel4utils/sel4_zf_logif.h>
#include <platsupport/io.h>

#include "pico_common.h"

#define PICO_TICK_MS 10

/*
 * WARNING: Currently, malloc in muslc is broken in the fact that if you try to malloc over the heap
 * limit, it will not fail gracefully. This is due to function calls that are performed by malloc trying to
 * set errno which we do not support. If at some point you encounter a data fault from trying to malloc, this
 * is the reason why.
 *
 * The limit with 0x40000 heap size limit is roughly 530 sockets, this does not include the frames allocated
 * by PicoTCP.
 */


seL4_CPtr ethdriver_notification(void);

/*
 * Needed for the picotcp from pico_sel4.h
 * Although time is not needed for UDP, it is used in the arp update.
 */
volatile uint32_t pico_ms_tick = 0;

/*
 * Required for the camkes_sys_clock_gettime() in sys_clock.c of libsel4camkes.
 */
int clk_get_time(void)
{
    uint64_t time_in_ms = pico_ms_tick;
    return (time_in_ms & 0xFFFFFFFF);
}

/* Callback that gets called when the timer fires. */
void timer_complete_callback(void)
{
    pico_ms_tick += PICO_TICK_MS;
    pico_stack_tick();
}

int pico_server_init(ps_io_ops_t *io_ops)
{
    eth_init();
    /* Start the timer for the TCP stack */
    timer_periodic(0, NS_IN_MS * PICO_TICK_MS);
    single_threaded_component_register_handler(timer_notification_badge(), timer_complete_callback, NULL);
}

CAMKES_PRE_INIT_MODULE_DEFINE(server_setup, pico_server_init);
