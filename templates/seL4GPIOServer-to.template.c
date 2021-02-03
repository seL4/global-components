/*#
 *#Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *#
 *#SPDX-License-Identifier: BSD-2-Clause
  #*/


#include <sel4/sel4.h>
#include <camkes.h>
#include <utils/util.h>
#include <platsupport/gpio.h>
#include <platsupport/plat/gpio.h>

/*- set client_ids = namespace() -*/
/*- if me.parent.type.to_threads == 0 -*/
    /*- include 'seL4RPCNoThreads-to.template.c' -*/
    /*- from 'global-endpoint.template.c' import allocate_rpc_endpoint_cap with context -*/
    /*- do allocate_rpc_endpoint_cap(me.parent.to_end, is_reader=True, other_end=me.parent.from_ends[0]) -*/
    /*- set client_ids.badges = pop('badges') -*/
/*- else -*/
    /*- include 'seL4RPCCall-to.template.c' -*/
    /*- from 'rpc-connector.c' import allocate_badges with context -*/
    /*- do allocate_badges(client_ids) -*/
/*- endif -*/

/*- set all_allocated_pins = [] -*/
/*- set client_allocated_pins = {} -*/
/*- for client_end in me.parent.from_ends -*/
    /*- set client_pin_configs = configuration[client_end.instance.name].get('%s_pins' % client_end.interface.name) -*/
    /*- if client_pin_configs is not none -*/
        /*- set client_pin_query = client_pin_configs.get('query') -*/
        /*- if client_pin_query is none -*/
            /*? raise(TemplateError('Missing GPIO allocation request, please assign GPIO IDs to the GPIO query instance')) ?*/
        /*- endif -*/
        /*- if client_pin_query[0] is none -*/
            /*? raise(TemplateError('Missing GPIO allocation request, please assign GPIO IDs to the GPIO query instance')) ?*/
        /*- endif -*/
        /*- set client_pins = client_pin_query[0]['desired_pins'] -*/
        /*- for pin in client_pins -*/
            /*- if pin in all_allocated_pins -*/
                /*? raise(TemplateError('Pin %s is being requested by two or more clients!' % pin)) ?*/
            /*- endif -*/
        /*- endfor -*/
        /*- do all_allocated_pins.extend(client_pins) -*/
        /*- set badge = client_ids.badges[loop.index0] -*/
        /*- do client_allocated_pins.setdefault(badge, []).extend(client_pins) -*/
            static gpio_id_t /*? me.interface.name ?*/_/*? badge ?*/_pins[] = {
                /*- for pin in client_pins -*/
                    /*? pin ?*/,
                /*- endfor -*/
                -1
            };
    /*- endif -*/
/*- endfor -*/

gpio_id_t */*? me.interface.name ?*/_get_client_pins(seL4_Word badge)
{
    switch (badge) {
        /*- for badge in client_allocated_pins.keys() -*/
        case /*? badge ?*/:
            return /*? me.interface.name ?*/_/*? badge ?*/_pins;
        /*- endfor -*/
        default:
            return NULL;
    }

    assert(!"Should never get here!");
}

unsigned int /*? me.interface.name ?*/_num_client_pins(seL4_Word badge)
{
    switch (badge) {
        /*- for badge in client_allocated_pins.keys() -*/
        case /*? badge ?*/:
            return /*? len(client_allocated_pins[badge]) ?*/;
        /*- endfor -*/
        default:
            return 0;
    }

    assert(!"Should never get here!");
}

seL4_Word /*? me.interface.name ?*/_get_pin_assignee(gpio_id_t pin_id)
{
    switch (pin_id) {
        /*- for badge in client_allocated_pins.keys() -*/
            /*- for pin in client_allocated_pins[badge] -*/
        case /*? pin ?*/:
            return /*? badge ?*/;
            /*- endfor -*/
        /*- endfor -*/
        default:
            return 0;
    }

    assert(!"Should never get here!");
}
