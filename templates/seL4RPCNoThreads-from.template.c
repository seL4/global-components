/*
 * Copyright 2020, Data61, ABN 41 687 119 230.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*- from 'rpc-connector.c' import establish_from_rpc, begin_send, perform_call, release_recv with context -*/
/*- from 'global-endpoint.template.c' import allocate_rpc_endpoint_cap with context -*/
#include <camkes/dataport.h>

/*? macros.show_includes(me.instance.type.includes) ?*/
/*? macros.show_includes(me.interface.type.includes) ?*/

/*- set connector = namespace() -*/

/*- set connector.language = 'c' -*/
/*# Establish the buffer for message contents #*/
/*- set connector.userspace_ipc = False -*/
/*- set base = '((void*)&seL4_GetIPCBuffer()->msg[0])' -*/
/*- set buffer_size = '(seL4_MsgMaxLength * sizeof(seL4_Word))' -*/
/*- set connector.lock = False -*/
/*- set connector.send_buffer = base -*/
/*- set connector.recv_buffer = base -*/
/*- set connector.send_buffer_size = buffer_size -*/
/*- set connector.recv_buffer_size = buffer_size -*/
/*- set connector.recv_buffer_size_fixed = connector.userspace_ipc -*/

/*# Ensure the endpoint is allocated #*/
/*- do allocate_rpc_endpoint_cap(me.parent.to_end, is_reader=False, other_end=me) -*/
/*- set connector.badges = pop('badges') -*/
/*- set connector.ep = pop('endpoint') -*/


/*- include 'rpc-connector-common-from.c' -*/
