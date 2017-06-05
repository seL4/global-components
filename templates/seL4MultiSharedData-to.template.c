/*
 * Copyright 2017, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */

#include <camkes/dataport.h>
#include <utils/util.h>

/*? macros.show_includes(me.to_instance.type.includes) ?*/

/*# Enumerate all the incoming interfaces #*/
/*- set dataports = [] -*/
/*- set client_ids = set() -*/
/*- for id, c in enumerate(composition.connections) -*/
    /*- if c.to_instance.name == me.to_instance.name and c.to_interface.name == me.to_interface.name -*/
        /*- if c.type.name == me.type.name -*/
            /*- set client_id = configuration[c.from_instance.name].get("%s_attributes" % c.from_interface.name) -*/
            /*- if client_id is none or re.match('"\\d+"$', client_id) is none -*/
              /*? raise(Exception('%s.%s_attributes must be set to a number' % (c.from_instance.name, c.from_interface.name))) ?*/
            /*- endif -*/
            /*- set client_id = client_id.strip('"') -*/
            /*- if client_id not in client_ids -*/
                /*- do client_ids.add(client_id) -*/
                /*- set dataport_name = '%s_%s' % (c.from_interface.name, client_id) -*/
                /*- set dataport_type = configuration[c.from_instance.name].get("%s_dataport_type" % c.from_interface.name, 'Buf') -*/
                /*- set dataport_section = '%s_%s' % (me.to_interface.name, client_id) -*/
                /*- set p = Perspective(dataport=dataport_name) -*/
                #define SHM_ALIGN (1 << 12)
                struct {
                    char content[ROUND_UP_UNSAFE(sizeof(/*? dataport_type ?*/),
                        PAGE_SIZE_4K)];
                } /*? p['dataport_symbol'] ?*/
                        __attribute__((aligned(SHM_ALIGN)))
                        __attribute__((section("shared_/*? dataport_section ?*/")))
                        __attribute__((externally_visible));

                volatile /*? dataport_type ?*/ * /*? me.to_interface.name ?*/_buf_/*? client_id ?*/ =
                    (volatile /*? dataport_type ?*/ *) & /*? p['dataport_symbol'] ?*/;
                /*- do dataports.append( ('%s_buf_%s' % (me.to_interface.name, client_id), client_id) ) -*/
            /*- else -*/
                /* skipping /*? client_id ?*/ */
            /*- endif -*/
        /*- endif -*/
    /*- endif -*/
/*- endfor -*/

void */*? me.to_interface.name ?*/_buf(unsigned int client_id) {
    /*- if len(dataports) == 0 -*/
        return NULL;
    /*- else -*/
        switch (client_id) {
            /*- for symbol, id in dataports -*/
            case /*? id ?*/:
                return (void*)/*? symbol ?*/;
            /*- endfor -*/
        default:
            return NULL;
        }
    /*- endif -*/
}

unsigned int /*? me.to_interface.name ?*/_num_badges(void) {
    return /*? len(dataports) ?*/;
}

unsigned int /*? me.to_interface.name ?*/_enumerate_badge(unsigned int i) {
    /*- if len(dataports) == 0 -*/
        return -1;
    /*- else -*/
        switch (i) {
            /*- for _, id in dataports -*/
                case /*? loop.index0 ?*/:
                    return /*? id ?*/;
            /*- endfor -*/
        default:
            return -1;
        }
    /*- endif -*/
}

/*- set isdata = lambda('x: isinstance(x, AST.Dataport)')(me.to_interface) -*/
/*- if isdata -*/
    /*- set p = Perspective(dataport=me.to_interface.name) -*/
    struct {
        char content[ROUND_UP_UNSAFE(sizeof(/*? show(me.to_interface.type) ?*/),
            PAGE_SIZE_4K)];
    } /*? p['dataport_symbol'] ?*/
            __attribute__((aligned(SHM_ALIGN)))
            __attribute__((section("shared_/*? me.to_interface.name ?*/")))
            __attribute__((externally_visible));
/*- endif -*/


int /*? me.to_interface.name ?*/_wrap_ptr(dataport_ptr_t *p, void *ptr) {
    return -1;
}

void * /*? me.to_interface.name ?*/_unwrap_ptr(dataport_ptr_t *p) {
    return NULL;
}

