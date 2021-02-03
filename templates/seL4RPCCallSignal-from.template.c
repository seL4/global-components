/*#
 *#Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *#
 *#SPDX-License-Identifier: BSD-2-Clause
  #*/


/*- if me.parent.type.to_threads == 0 -*/
    /*- include 'seL4RPCNoThreads-from.template.c' -*/
/*- else -*/
    /*- include 'seL4RPCCall-from.template.c' -*/
/*- endif -*/

/*- include 'get-notification.template.c' -*/
