/*#
 *# Copyright 2014, NICTA
 *#
 *# This software may be distributed and modified according to the terms of
 *# the BSD 2-Clause license. Note that NO WARRANTY is provided.
 *# See "LICENSE_BSD2.txt" for details.
 *#
 *# @TAG(NICTA_BSD)
 #*/

#include <camkes/dataport.h>
#include <utils/util.h>

/*? macros.show_includes(me.instance.type.includes) ?*/


/*- set client_id = configuration[me.instance.name].get("%s_attributes" % me.interface.name) -*/
/*- if client_id is none or re.match('\\d+$', client_id) is none -*/
  /*? raise(Exception('%s.%s_attributes must be set to a number' % (me.instance.name, me.interface.name))) ?*/
/*- endif -*/
/*- set client_id = client_id.strip('"') -*/

/*- if suffix is not defined -*/
  /*- set suffix = '' -*/
/*- endif -*/

/*- set shmem_size = configuration[me.instance.name].get("%s_shmem_size" % me.interface.name, 4096) -*/
/*- set shmem_section = 'from_%s' % me.interface.name -*/
/*- set shmem_symbol = 'from_%s_data' % me.interface.name -*/
/*- set shmem_name = '%s%s' % (me.interface.name, suffix) -*/

#define SHM_ALIGN (1 << 12)
struct {
    char content[ROUND_UP_UNSAFE(/*? shmem_size ?*/, PAGE_SIZE_4K)];
} /*? shmem_symbol ?*/
        __attribute__((aligned(SHM_ALIGN)))
        __attribute__((section("shared_/*? shmem_symbol ?*/")))
        __attribute__((externally_visible));

/*- do register_shared_variable('%s_%s_data' % (me.parent.name, client_id), shmem_symbol) -*/

volatile void * /*? shmem_name ?*/ = (volatile void *) & /*? shmem_symbol ?*/;

int /*? me.interface.name ?*/_wrap_ptr(dataport_ptr_t *p, void *ptr) {
    return -1;
}

void * /*? me.interface.name ?*/_unwrap_ptr(dataport_ptr_t *p) {
    return NULL;
}
