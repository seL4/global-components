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

/*# Assign client ids and badges #*/
/*- from 'rpc-connector.c' import allocate_badges with context -*/
/*- from 'global-endpoint.template.c' import allocate_cap with context -*/
/*- set client_ids = namespace() -*/
/*- do allocate_badges(client_ids) -*/

/*- set badges = client_ids.badges -*/

/*- for c in me.parent.from_ends -*/
    /*- do allocate_cap(c, is_reader=False) -*/
    /*- set notification = pop('notification') -*/
    void /*? me.interface.name ?*/_emit_/*? badges[loop.index0] ?*/(void) {
        seL4_Signal(/*? notification ?*/);
    }
/*- endfor -*/

/*- do badges.sort() -*/

void /*? me.interface.name ?*/_emit(unsigned int badge) {
    /*# create a lookup table under the assumption that the
        badges are sensibly made as low as possible #*/
    static void (*lookup[])(void) = {
        /*- for badge in badges -*/
            [/*? badge ?*/] = /*? me.interface.name ?*/_emit_/*? badge ?*/,
        /*- endfor -*/
    };
    assert(badge < ARRAY_SIZE(lookup));
    assert(lookup[badge]);
    lookup[badge]();
}

int /*? me.interface.name ?*/_largest_badge(void) {
    return /*? badges[len(badges) - 1] ?*/;
}
