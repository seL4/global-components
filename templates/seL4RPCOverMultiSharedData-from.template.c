/*#
 *#Copyright 2020, Data61
 *#Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 *#ABN 41 687 119 230.
 *#
 *#This software may be distributed and modified according to the terms of
 *#the BSD 2-Clause license. Note that NO WARRANTY is provided.
 *#See "LICENSE_BSD2.txt" for details.
 *#
 *#@TAG(DATA61_BSD)
  #*/
/*- set suffix = "_buf" -*/
/*- include 'seL4MultiSharedData-from.template.c' -*/

/*- from 'rpc-connector.c' import establish_from_rpc, begin_send, perform_call, release_recv with context -*/

#include <camkes/dataport.h>

/*? macros.show_includes(me.instance.type.includes) ?*/
/*? macros.show_includes(me.interface.type.includes) ?*/

/*- set connector = namespace() -*/
/*- set shmem_size = configuration[me.instance.name].get("%s_shmem_size" % me.interface.name, 4096) -*/

/*- set lock = True -*/
/*? establish_from_rpc(connector, buffer=('((void*)%s%s)' % (me.interface.name, suffix), shmem_size, lock)) ?*/

/*- include 'rpc-connector-common-from.c' -*/
