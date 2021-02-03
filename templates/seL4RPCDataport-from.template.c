/*#
 *#Copyright 2017, Data61, CSIRO (ABN 41 687 119 230)
 *#
 *#SPDX-License-Identifier: BSD-2-Clause
  #*/


/*- set client_ids = namespace() -*/
/*- if me.parent.type.to_threads == 0 -*/
    /*- include 'seL4RPCNoThreads-from.template.c' -*/
    /*- from 'global-endpoint.template.c' import allocate_rpc_endpoint_cap with context -*/
    /*- do allocate_rpc_endpoint_cap(me.parent.to_end, is_reader=False, other_end=me) -*/
    /*- set client_ids.badges = pop('badges') -*/

/*- else -*/
    /*- include 'seL4RPCCall-from.template.c' -*/
    /*- from 'rpc-connector.c' import allocate_badges with context -*/
    /*- do allocate_badges(client_ids) -*/
/*- endif -*/

/*- set suffix = "_buf" -*/
/*- include 'seL4MultiSharedData-from.template.c' -*/
