/*#
 *#Copyright 2019, Data61
 *#Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 *#ABN 41 687 119 230.
 *#
 *#This software may be distributed and modified according to the terms of
 *#the BSD 2-Clause license. Note that NO WARRANTY is provided.
 *#See "LICENSE_BSD2.txt" for details.
 *#
 *#@TAG(DATA61_BSD)
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
