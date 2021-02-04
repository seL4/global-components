/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sel4/sel4.h>

seL4_CPtr /*? me.interface.name ?*/_notification(void);

seL4_Word /*? me.interface.name ?*/_notification_badge(void);

static inline void /*? me.interface.name ?*/_wait_notification(void)
{
    seL4_Wait(/*? me.interface.name ?*/_notification(), NULL);
}
