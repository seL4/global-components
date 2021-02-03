/*#
 *#Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *#
 *#SPDX-License-Identifier: BSD-2-Clause
  #*/


/*- from 'global-endpoint.template.c' import allocate_cap with context -*/

/*- do allocate_cap(me, is_reader=True) -*/
/*- set notification = pop('notification') -*/
/*- set badge = pop('badge') -*/

/*# This is only used for seL4GlobalAsynchCallback #*/
/*- do stash('callback_notification', notification) -*/

seL4_CPtr /*? me.interface.name ?*/_notification(void) {
    return /*? notification ?*/;
}

seL4_Word /*? me.interface.name ?*/_notification_badge(void) {
    return /*? badge ?*/;
}
