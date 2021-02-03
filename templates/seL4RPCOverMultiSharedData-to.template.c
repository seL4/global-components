/*#
 *#Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *#
 *#SPDX-License-Identifier: BSD-2-Clause
  #*/

/*- include 'seL4MultiSharedData-to.template.c' -*/


/*- from 'rpc-connector.c' import establish_recv_rpc, recv_first_rpc, complete_recv, begin_recv, begin_reply, complete_reply, reply_recv with context -*/

#include <camkes/dataport.h>

/*? macros.show_includes(me.instance.type.includes) ?*/
/*? macros.show_includes(me.interface.type.includes) ?*/

/*- set connector = namespace() -*/

/*? establish_recv_rpc(connector, me.interface.name, buffer=('(%s_buf(%s_get_sender_id()))' % (me.interface.name,me.interface.name) , '(%s_buf_size(%s_get_sender_id()))' % (me.interface.name,me.interface.name))) ?*/

/*- include 'rpc-connector-common-to.c' -*/

