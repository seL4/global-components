/*#
 *#Copyright 2017, Data61
 *#Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 *#ABN 41 687 119 230.
 *#
 *#This software may be distributed and modified according to the terms of
 *#the BSD 2-Clause license. Note that NO WARRANTY is provided.
 *#See "LICENSE_BSD2.txt" for details.
 *#
 *#@TAG(DATA61_BSD)
  #*/
#include <sel4/sel4.h>
/*- from 'global-endpoint.template.c' import allocate_cap with context -*/

/*? macros.show_includes(me.instance.type.includes) ?*/

/*- for c in me.parent.to_ends -*/
    /*- do allocate_cap(c, is_reader=False) -*/
    /*- set notification = pop('notification') -*/

    void /*? me.interface.name ?*/_emit_underlying(void) {
        seL4_Signal(/*? notification ?*/);
    }
/*- endfor -*/
