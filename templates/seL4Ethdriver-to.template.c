/*
 * Copyright 2017, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdbool.h>

/*- include 'seL4RPCDataport-to.template.c' -*/

/*# Assign client ids and badges #*/
/*- from 'rpc-connector.c' import allocate_badges with context -*/
/*- from 'global-endpoint.template.c' import allocate_cap with context -*/
/*- set client_ids = namespace() -*/
/*- do allocate_badges(client_ids) -*/
/*- set badges = client_ids.badges -*/

/*- set macs = [] -*/

/*- for c in me.parent.from_ends -*/
    /*- do allocate_cap(c, is_reader=False) -*/
    /*- set notification = pop('notification') -*/
    /*- set badge = badges[loop.index0] -*/
    /*- set mac = configuration[c.instance.name].get("%s_mac" % c.interface.name) -*/
    void /*? me.interface.name ?*/_emit_/*? badge ?*/(void) {
        seL4_Signal(/*? notification ?*/);
    }
    /*- do macs.append( (badge, mac) ) -*/
/*- endfor -*/

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

bool /*? me.interface.name ?*/_has_mac(unsigned int badge) {
    /*- if len(macs) > 0 -*/
        switch (badge) {
            /*- for badge,mac in macs -*/
            case /*? badge ?*/: {
                /*- if mac != None -*/
                    return true;
                /*- else -*/
                    return false;
                /*- endif -*/
            }
            /*- endfor -*/
        }
    /*- endif -*/
    assert(0);
}

void /*? me.interface.name ?*/_get_mac(unsigned int badge, uint8_t *mac) {
    /*- if len(macs) > 0 -*/
        switch (badge) {
            /*- for badge,mac in macs -*/
                /*- if mac == None -*/
                    /*- continue -*/
                /*- endif -*/
            case /*? badge ?*/: {
                uint8_t temp[] = {
                    /*- for num in mac -*/
                        /*? num ?*/,
                    /*- endfor -*/
                };
                memcpy(mac, temp, sizeof(temp));
                break;
            }
            /*- endfor -*/
            default:
                break;
        }
    /*- endif -*/
}
