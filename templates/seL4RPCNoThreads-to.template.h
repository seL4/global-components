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


/*- from 'rpc-connector.c' import establish_recv_rpc, recv_first_rpc, complete_recv, begin_recv, begin_reply, complete_reply, reply_recv with context -*/
/*- from 'global-endpoint.template.c' import allocate_rpc_endpoint_cap with context -*/

/*# Ensure the endpoint is allocated #*/
/*- do allocate_rpc_endpoint_cap(me, is_reader=True, other_end=me.parent.from_ends[0]) -*/
/*- set badges = pop('badges') -*/
/*- set ep = pop('endpoint') -*/

#define /*? me.interface.name ?*/_num_clients() /*? len(badges) ?*/

#define /*? me.interface.name ?*/_BADGES_MACRO_X_ITERATOR() \
/*- for b in badges --*/ 
	X(/*?b?*/) \
/*- endfor -*/

int /*? me.interface.name ?*/_handle_message(seL4_MessageInfo_t *info, seL4_Word badge);
