/*
 * Copyright 2018, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*- from 'global-endpoint.template.c' import allocate_cap with context -*/

#include <sel4/sel4.h>
#include <camkes/dataport.h>
#include <camkes.h>
#include <camkes/virtqueue_template.h>
#include <virtqueue.h>

/*- if len(me.parent.to_ends) != 1 -*/
    /*? raise(Exception('%s must only have 1 to end' % (me.parent.name))) ?*/
/*- endif -*/
/*- set to_end = me.parent.to_ends[0] -*/

/*- set all_connection_ends = me.parent.from_ends -*/

/*- set interface_name =  me.interface.type.name -*/

/*- if interface_name == "VirtQueueDrv" -*/
    /*- set end_string = "drv" -*/
    /*- set other_end_string = "dev" -*/
/*- else -*/
    /*- set end_string = "dev" -*/
    /*- set other_end_string = "drv" -*/
/*- endif -*/

/*- set topology = configuration[to_end.instance.name].get("%s_topology" % to_end.interface.name, []) -*/
/*- if len(topology) == 0 -*/
    /*- if len(all_connection_ends) > 2 -*/
        /*? raise(Exception("%s: Has more than 2 ends and could not find 'topology' definition describing how to connect them." % (me.parent.name))) ?*/
    /*- else -*/
        /*- do topology.append({}) -*/
        /*- for end in all_connection_ends -*/
            /*- if end == me -*/
                /*- do topology[0].update({end_string: str(end)}) -*/
            /*- else -*/
                /*- do topology[0].update({other_end_string: str(end)}) -*/
            /*- endif -*/
        /*- endfor -*/

    /*- endif -*/
/*- endif -*/

/*- set topology_entry = [] -*/

/*- for entry in topology -*/
    /*- if entry[end_string] == "%s.%s" % (me.instance.name, me.interface.name) -*/
        /*- do topology_entry.append(entry) -*/
    /*- endif -*/
/*- endfor -*/

/*- if len(topology_entry) != 1 -*/
    /*? raise(Exception('Could not find topology entry for: %s.%s' % (me.instance.name, me.interface.name))) ?*/
/*- endif -*/

/*# Check that there is a valid interface on the other end of the topology from us #*/
/*- set other_interface_name = topology_entry[0][other_end_string] -*/
/*- set other_interface = [0] -*/
/*- for c in all_connection_ends -*/
  /*- if str(c) == other_interface_name -*/
    /*- do other_interface.__setitem__(0, c) -*/
  /*- endif -*/
/*- endfor -*/
/*- if other_interface[0] == 0 -*/
    /*? raise(TemplateError('Interface %s is not present in connection.' % (other_interface_name))) ?*/
/*- endif -*/
/*- set other_interface = other_interface[0] -*/
/*- set queue_length = configuration[me.parent.name].get("queue_length", 256) -*/
/*- if queue_length is none or not isinstance(queue_length, six.integer_types) -*/
    /*? raise(Exception('%s.queue_length must be set to a number' % (me.parent.name))) ?*/
/*- endif -*/


/*# Create shared memory region between the two interfaces #*/
/*- set shmem_size = configuration[me.instance.name].get("%s_shmem_size" % me.interface.name, 8192) -*/
/*- if shmem_size != configuration[other_interface.instance.name].get("%s_shmem_size" % other_interface.interface.name, 8192) -*/
    /*? raise(TemplateError('Setting %s.%s_shmem_size does not match size configuration from other side: %d vs. %d' % (me.instance.name, me.interface.name, shmem_size, configuration[other_interface.instance.name].get("%s_shmem_size" % other_interface.interface.name, 4096)))) ?*/
/*- endif -*/
/*- if end_string == 'drv' -*/
    /*- set shmem_symbol = '%s_%s_data' % (str(me), other_interface_name) -*/
/*- else -*/
    /*- set shmem_symbol = '%s_%s_data' % (other_interface_name, str(me)) -*/
/*- endif -*/
/*- set shmem_symbol = shmem_symbol.replace('.', '_') -*/
/*- set page_size = macros.get_page_size(shmem_size, options.architecture) -*/
/*- if page_size == 0 -*/
  /*? raise(TemplateError('Setting %s.%s_shmem_size does not meet minimum size and alignment requirements. %d must be at least %d and %d aligned' % (me.instance.name, me.interface.name, size, 4096, 4096))) ?*/
/*- endif -*/

/*? macros.shared_buffer_symbol(sym=shmem_symbol, shmem_size=shmem_size, page_size=page_size) ?*/
/*? register_shared_variable(shmem_symbol, shmem_symbol, shmem_size, frame_size=page_size) ?*/

size_t /*? me.interface.name ?*/_get_size(void) {
    return sizeof(/*? shmem_symbol ?*/);
}


/*# We need to create a notification badge of their notificaion in our cspace #*/
/*- do allocate_cap(other_interface, is_reader=False) -*/
/*- set notification = pop('notification') -*/

/*- set disable_optimizations = configuration[me.interface.name].get("disable_optimizations", 0) -*/

/*- if configuration[me.instance.name].get('single_threaded', 0) and disable_optimizations != 1 -*/
extern seL4_CPtr signal_to_send;
static void /*? me.interface.name ?*/_notify(void) {
    if (signal_to_send) {
        seL4_Signal(signal_to_send);
    }
    signal_to_send = /*? notification ?*/;

}
/*- else -*/
static void /*? me.interface.name ?*/_notify(void) {
    seL4_Signal(/*? notification ?*/);
}

/*- endif -*/

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

/*- set queue_id = macros.virtqueue_get_client_id(composition, me, configuration) -*/
/*- if queue_id is none or not isinstance(queue_id, six.integer_types) -*/
  /*? raise(Exception('%s.%s_id must be set to a number' % (me.instance.name, me.interface.name))) ?*/
/*- endif -*/

//This is called by camkes runtime during init.
static void __attribute__((constructor)) register_connector(void) {
/*- if interface_name == "VirtQueueDrv" -*/
    camkes_virtqueue_channel_register(/*? queue_id ?*/, "/*? me.interface.name ?*/", /*? queue_length ?*/, /*? me.interface.name ?*/_get_size(), (volatile void *) &/*? shmem_symbol ?*/,  /*? me.interface.name ?*/_notify, /*? me.interface.name ?*/_notification(), /*? me.interface.name ?*/_notification_badge(), VIRTQUEUE_DRIVER);
/*- else -*/
    camkes_virtqueue_channel_register(/*? queue_id ?*/, "/*? me.interface.name ?*/", /*? queue_length ?*/, /*? me.interface.name ?*/_get_size(), (volatile void *) &/*? shmem_symbol ?*/,  /*? me.interface.name ?*/_notify, /*? me.interface.name ?*/_notification(), /*? me.interface.name ?*/_notification_badge(), VIRTQUEUE_DEVICE);
/*- endif -*/
}
