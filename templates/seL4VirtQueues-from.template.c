/*
 * Copyright 2018, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */

/*- from 'global-endpoint.template.c' import allocate_cap with context -*/

#include <sel4/sel4.h>
#include <camkes/dataport.h>
#include <camkes.h>
#include <camkes/virtqueue_template.h>
#include <virtqueue.h>

/*- set suffix = "_buf" -*/
/*- include 'seL4MultiSharedData-from.template.c' -*/

/*- if len(me.parent.to_ends) != 1 -*/
    /*? raise(Exception('%s must only have 1 to end' % (me.parent.name))) ?*/
/*- endif -*/
/*- set to_end = me.parent.to_ends[0] -*/

/*- set interface_name =  me.interface.type.name -*/

/*- if interface_name == "VirtQueueDrv" -*/
    /*- set end_string = "drv" -*/
    /*- set other_end_string = "dev" -*/
/*- else -*/
    /*- set end_string = "dev" -*/
    /*- set other_end_string = "drv" -*/
/*- endif -*/

/*- set topology = configuration[to_end.instance.name].get("%s_topology" % to_end.interface.name) -*/
/*- set topology_entry = [] -*/

/*- for entry in topology -*/
    /*- if entry[end_string] == "%s.%s" % (me.instance.name, me.interface.name) -*/
        /*- do topology_entry.append(entry) -*/
    /*- endif -*/
/*- endfor -*/

/*- if len(topology_entry) != 1 -*/
    /*? raise(Exception('Could not find topology entry for: %s.%s' % (me.instance.name, me.interface.name))) ?*/
/*- endif -*/

/*- set other_interface_name = topology_entry[0][other_end_string] -*/

/*# We need to create a notification badge of their notificaion in our cspace #*/

/*- for c in me.parent.from_ends -*/
  /*- if str(c) == other_interface_name -*/

    /*- do allocate_cap(c, is_reader=False) -*/
    /*- set notification = pop('notification') -*/

    static void /*? me.interface.name ?*/_notify(void) {
        seL4_Signal(/*? notification ?*/);
    }
  /*- endif -*/
/*- endfor -*/

/*# We need to get the badge that they will signal us on #*/
/*- do allocate_cap(me, is_reader=True) -*/
/*- set notification = pop('notification') -*/
/*- set badge = pop('badge') -*/

static seL4_CPtr /*? me.interface.name ?*/_notification(void) {
    return /*? notification ?*/;
}

seL4_Word /*? me.interface.name ?*/_notification_badge(void) {
    return /*? badge ?*/;
}

/*- set interface_name =  me.interface.type.name -*/

/*- set queue_id = configuration[me.instance.name].get("%s_id" % me.interface.name) -*/
/*- if queue_id is none or not isinstance(queue_id, six.integer_types) -*/
  /*? raise(Exception('%s.%s_id must be set to a number' % (me.instance.name, me.interface.name))) ?*/
/*- endif -*/

//This is called by camkes runtime during init.
static void __attribute__((constructor)) register_connector(void) {
/*- if interface_name == "VirtQueueDrv" -*/
    camkes_virtqueue_channel_register(/*? queue_id ?*/, /*? me.interface.name ?*/_get_size(), /*? me.interface.name ?*/_buf,  /*? me.interface.name ?*/_notify, /*? me.interface.name ?*/_notification(), /*? me.interface.name ?*/_notification_badge(), VIRTQUEUE_DRIVER);
/*- else -*/
    camkes_virtqueue_channel_register(/*? queue_id ?*/, /*? me.interface.name ?*/_get_size(), /*? me.interface.name ?*/_buf,  /*? me.interface.name ?*/_notify, /*? me.interface.name ?*/_notification(), /*? me.interface.name ?*/_notification_badge(), VIRTQUEUE_DEVICE);
/*- endif -*/
}
