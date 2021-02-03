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

/*# Message Type #*/

/*- set type = macros.dataport_type(me.interface.type) -*/
/*- set type_size_macro = macros.dataport_size(me.interface.type) -*/

/*# Sanity check to make sure that there is only one receiver connected #*/
/*- if len(me.parent.to_ends) != 1 -*/
    /*? raise(Exception('MessageQueue for %s.%s is missing a receiver or has more than one receiver' % (me.instance.name, me.interface.name))) ?*/
/*- endif -*/

/*- set receiver = me.parent.to_ends[0] -*/

/*# Notifications #*/

/*# Need a badge as we also want the clients to be able to poll #*/
/*- set receiver_ntfn = alloc_obj('%s_%s_receiver_ntfn' % (me.instance.name, me.interface.name), seL4_NotificationObject) -*/
/*- set receiver_ntfn_cap = alloc_cap('%s_%s_receiver_ntfn' % (me.instance.name, me.interface.name), receiver_ntfn, read=True, write=True, badge=1) -*/

/*# Queue Size #*/

/*- set queue_size = configuration[me.parent.name].get("queue_size", 128) -*/
/*- set virtqueue_length = 2 * queue_size -*/

/*- set queue_total_size = '%s * %s + sizeof(vq_vring_avail_t) + sizeof(uint16_t) * %s + sizeof(vq_vring_used_t) + sizeof(struct vq_vring_used_elem) * %s  + sizeof(vq_vring_desc_t) * %s' % (type_size_macro, queue_size, virtqueue_length, virtqueue_length, virtqueue_length) -*/
/*- set index = me.parent.from_ends.index(me) -*/
/*# TODO This isn't so resilient, maybe stash the name of the buffer? #*/
/*- set single_shmem_size = 'sizeof(from_%d_%s_data)' % (index, me.interface.name) -*/

/*- set interface_name =  me.interface.type.name -*/

/*- set queue_id = configuration[me.instance.name].get("%s_id" % me.interface.name) -*/
/*- if queue_id is none or not isinstance(queue_id, six.integer_types) -*/
    /*? raise(Exception('%s.%s_id must be set to a number and should be unique across seL4MessageQueue connections in an instance ' % (me.instance.name, me.interface.name))) ?*/
/*- endif -*/

static_assert(/*? queue_total_size ?*/ <= /*? single_shmem_size ?*/,
              "Queue size is too large for msgqueue /*? receiver.instance.name ?*/./*? receiver.interface.name ?*/,"
              " consider increasing the size of the buffer");

static void /*? me.interface.name ?*/_notify(void)
{
    seL4_Signal(/*? receiver_ntfn_cap ?*/);
}

void /*? me.interface.name ?*/__init()
{
    int error = 0;
    error = camkes_msgqueue_channel_register_sender(/*? queue_id ?*/,
                                                    /*? me.interface.name ?*/,
                                                    /*? virtqueue_length ?*/,
                                                    /*? single_shmem_size ?*/,
                                                    /*? type_size_macro ?*/,
                                                    /*? me.interface.name ?*/_notify);
    if (error) {
        assert(!"Failed to initialise the msgqueue for connection /*? me.instance.name ?*/./*? me.interface.name ?*/");
    }
}
