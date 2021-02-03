/*#
 *#Copyright 2017, Data61, CSIRO (ABN 41 687 119 230)
 *#
 *#SPDX-License-Identifier: BSD-2-Clause
  #*/

#include <sel4/sel4.h>

/*? macros.show_includes(me.instance.type.includes) ?*/

/*- include 'get-notification.template.c' -*/

/*- set callback_notification = pop('callback_notification') -*/

void /*? me.interface.name ?*/_callback(seL4_Word badge);

void /*? me.interface.name ?*/__run(void) {
    while(1) {
        seL4_Word badge;
        seL4_Wait(/*? callback_notification ?*/, &badge);
        /*? me.interface.name ?*/_callback(badge);
    }
}
