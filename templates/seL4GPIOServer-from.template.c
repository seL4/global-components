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
    /*- include 'seL4RPCNoThreads-from.template.c' -*/
    /*- from 'global-endpoint.template.c' import allocate_rpc_endpoint_cap with context -*/
    /*- do allocate_rpc_endpoint_cap(me.parent.to_end, is_reader=False, other_end=me) -*/
    /*- set client_ids.badges = pop('badges') -*/
/*- else -*/
    /*- include 'seL4RPCCall-from.template.c' -*/
    /*- from 'rpc-connector.c' import allocate_badges with context -*/
    /*- do allocate_badges(client_ids) -*/
/*- endif -*/

/*- set allocated_pins = [] -*/
/*- set pin_config = configuration[me.instance.name].get('%s_pins' % me.interface.name) -*/
/*- if pin_config is not none -*/
    /*- set pin_query = pin_config.get('query') -*/
    /*- if pin_query is none -*/
        /*? raise(TemplateError('Missing GPIO allocation request, please assign GPIO IDs to the GPIO query instance')) ?*/
    /*- endif -*/
    /*- if pin_query[0] is none -*/
        /*? raise(TemplateError('Missing GPIO allocation request, please assign GPIO IDs to the GPIO query instance')) ?*/
    /*- endif -*/
    /*- do allocated_pins.extend(pin_query[0]['desired_pins']) -*/
/*- endif -*/

/*# Check if all the requested pins are actual pins on the platform #*/
/*- for pin in allocated_pins -*/
    /*- if pin not in pin_config['all_pins'] -*/
        /*? raise(TemplateError('Pin %s is not an actual pin on the platform!' % pin)) ?*/
    /*- endif -*/
/*- endfor -*/

static gpio_id_t /*? me.interface.name ?*/_pins[] = {
    /*- for pin in allocated_pins -*/
        /*? '%s,' % pin ?*/
    /*- endfor -*/
    -1,
};

gpio_id_t */*? me.interface.name ?*/_get_allocated_pins(void)
{
    return /*? me.interface.name ?*/_pins;
}

unsigned int /*? me.interface.name ?*/_num_allocated_pins(void)
{
    return /*? len(allocated_pins) ?*/;
}
