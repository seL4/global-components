/*#
 *#Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *#
 *#SPDX-License-Identifier: BSD-2-Clause
  #*/


#include <sel4/sel4.h>
#include <camkes.h>
#include <camkes/msgqueue.h>
#include <camkes/msgqueue_template.h>
#include <camkes/virtqueue_template.h>
#include <virtqueue.h>
#include <utils/util.h>

/*- include 'seL4SharedData.template.c' -*/

/*# Do a sanity check to make sure there are no two senders #*/
/*- if len(me.parent.from_ends) != 1 -*/
    /*? raise(Exception('MessageQueue for %s.%s is missing a sender or has more than one sender' % (me.instance.me, me.interface.name))) ?*/
/*- endif -*/

/*- set from_instance_name = me.parent.from_ends[0].instance.name -*/
/*- set from_interface_name = me.parent.from_ends[0].interface.name -*/

/*# Grab the notification, that the sender allocated or allocate it for the sender #*/
/*- set receiver_ntfn = alloc_obj('%s_%s_receiver_ntfn' % (from_instance_name, from_interface_name), seL4_NotificationObject) -*/
/*- set receiver_ntfn_cap = alloc_cap('%s_%s_receiver_ntfn_cap' % (from_instance_name, from_interface_name), receiver_ntfn, read=True, write=True, badge=1) -*/

/*- set type = macros.dataport_type(me.interface.type) -*/
/*- set type_size_macro = macros.dataport_size(me.interface.type) -*/

/*- set queue_id = configuration[me.instance.name].get("%s_id" % me.interface.name) -*/
/*- if queue_id is none or not isinstance(queue_id, six.integer_types) -*/
    /*? raise(Exception('%s.%s_id must be set to a number and should be unique across seL4MessageQueue connections in an instance ' % (me.instance.name, me.interface.name))) ?*/
/*- endif -*/

/*- set queue_size = configuration[me.parent.name].get("queue_size", 128) -*/
/*- set virtqueue_length = 2 * queue_size -*/

static int _/*? me.interface.name ?*/_poll()
{
    seL4_Word badge;
    seL4_Poll(/*? receiver_ntfn_cap ?*/, &badge);
    if (badge) {
        return 1;
    }
    return 0;
}

static void _/*? me.interface.name ?*/_wait()
{
    seL4_Wait(/*? receiver_ntfn_cap ?*/, NULL);
}

void /*? me.interface.name ?*/__init()
{
    int error = 0;
    error = camkes_msgqueue_channel_register_receiver(/*? queue_id ?*/,
                                                      /*? me.interface.name ?*/,
                                                      /*# TODO This isn't so resilient, maybe stash the name of the buffer? #*/
                                                      /*? virtqueue_length ?*/,
                                                      sizeof(/*? 'to_0_%s_data' % me.interface.name ?*/),
                                                      /*? type_size_macro ?*/,
                                                      _/*? me.interface.name ?*/_poll,
                                                      _/*? me.interface.name ?*/_wait);
    if (error) {
        assert(!"Failed to initialise the msgqueue for /*? me.instance.name ?*/./*? me.interface.name ?*/");
    }
}
