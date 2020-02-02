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

#include <stdbool.h>

/*
 * To prevent users from operating on the half-initialised picoserver, the picotcp
 * lock is held by the picoserver till the DHCP negotiation is finished.
 * Meanwhile, `dhcp_negotiating` is set to true, which allows `timer_complete_callback`
 * to involk callback functions without having to acquire the picotcp lock.
 * the lock will eventually be released after `eth_init_continue` is finished.
 * This is a **hacky** way to sync components.
 */
extern bool dhcp_negotiating;

uint32_t get_ipv4(void);
void eth_init(pico_device_eth *picotcp_driver);
