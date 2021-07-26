/*#
 *#Copyright 2017, Data61, CSIRO (ABN 41 687 119 230)
 *#
 *#SPDX-License-Identifier: BSD-2-Clause
  #*/


/*- macro allocate_cap(interface_end, is_reader) -*/
  /*? assert(isinstance(is_reader, bool)) ?*/

  /*- set instance = interface_end.instance -*/
  /*- if interface_end in interface_end.parent.from_ends -*/
    /*- set from_end = True -*/
    /*- set end = "from" -*/
  /*- else -*/
    /*- set from_end = False -*/
    /*- set end = "to" -*/
  /*- endif -*/

  /*- if interface_end.parent.type.get_attribute("%s_global_endpoint" % end).default != true -*/
      /*? raise(Exception('%s does not declare %s_global_endpoint:' % (interface_end.parent.type.name,end))) ?*/
  /*- endif -*/

  /*- set name = '%s_global_endpoint' % instance.name -*/
  /*- set config_name = configuration[instance.name].get('%s_global_endpoint' % interface_end.interface.name) -*/
  /*- if config_name and config_name != name -*/
    /*? raise(Exception('%s.%s_global_endpoint can no longer be set to an arbirtary value: %s %s' % (instance, interface_end.interface.name, config_name, name))) ?*/
  /*- endif -*/

  /*- set badge = macros.global_endpoint_badges(composition, interface_end, configuration, options.architecture) -*/
  /*- set stash_name = "%s_global_notification" % (name) -*/

  /*# Check the global stash for our endpoint #*/
  /*- set maybe_notification = _pop(stash_name) -*/

  /*# Create the endpoint if we need to #*/
  /*- if maybe_notification is none -*/
          /*- set notification_owner = instance.name -*/
          /*- set notification_object = alloc_obj(name, seL4_NotificationObject, label=notification_owner) -*/
  /*- else -*/
      /*- set notification_object, notification_owner = maybe_notification -*/
  /*- endif -*/

  /*# Put it back into the stash #*/
  /*- do _stash(stash_name, (notification_object, notification_owner)) -*/

  /*# Create the badged endpoint #*/
  /*- set notification = alloc_cap('%s_%s_%d_notification_object_cap' % (name, badge, is_reader), notification_object, read=is_reader, write=True) -*/
  /*- if not is_reader -*/
    /*- do cap_space.cnode[notification].set_badge(badge) -*/
  /*- endif -*/
  /*- do stash('notification', notification) -*/
  /*- do stash('badge', badge) -*/
/*- endmacro -*/

/*- macro allocate_cap_instance(instance) -*/

  /*- set name = '%s_global_endpoint' % instance.name -*/
  /*- set stash_name = "%s_global_notification" % (name) -*/

  /*# Check the global stash for our endpoint #*/
  /*- set maybe_notification = _pop(stash_name) -*/

  /*# Create the endpoint if we need to #*/
  /*- if maybe_notification is none -*/
          /*- set notification_owner = instance.name -*/
          /*- set notification_object = alloc_obj(name, seL4_NotificationObject, label=notification_owner) -*/
  /*- else -*/
      /*- set notification_object, notification_owner = maybe_notification -*/
  /*- endif -*/

  /*# Put it back into the stash #*/
  /*- do _stash(stash_name, (notification_object, notification_owner)) -*/

  /*- set notification = alloc_cap('%s_read_notification_object_cap' % (name), notification_object, read=True, write=True) -*/
  /*- do stash('notification', notification) -*/

/*- endmacro -*/

/*- macro allocate_rpc_endpoint_cap(interface_end, is_reader, other_end) -*/
  /*? assert(isinstance(is_reader, bool)) ?*/

  /*- set instance = interface_end.instance -*/
  /*- if interface_end in interface_end.parent.from_ends -*/
    /*- set from_end = True -*/
    /*- set end = "from" -*/
    /*- set index = interface_end.parent.from_ends.index(interface_end) -*/
    /*- set other_index = interface_end.parent.to_ends.index(other_end) -*/
  /*- else -*/
    /*- set from_end = False -*/
    /*- set end = "to" -*/
    /*- set index = interface_end.parent.to_ends.index(interface_end) -*/
    /*- set other_index = interface_end.parent.from_ends.index(other_end) -*/
  /*- endif -*/

  /*- if interface_end.parent.type.get_attribute("%s_global_rpc_endpoint" % end).default != true -*/
      /*? raise(Exception('%s does not declare %s_global_rpc_endpoint:' % (interface_end.parent.type.name,end))) ?*/
  /*- endif -*/


  /*- set name = '%s_global_rpc_endpoint' % instance.name -*/

  /*- set badges = macros.global_rpc_endpoint_badges(composition, interface_end, configuration) -*/
  /*- set stash_name = "%s_global_rpc_endpoint" % (name) -*/

  /*# Check the global stash for our endpoint #*/
  /*- set maybe_object = _pop(stash_name) -*/

  /*# Create the endpoint if we need to #*/
  /*- if maybe_object is none -*/
          /*- set endpoint_owner = instance.name -*/
          /*- set object = alloc_obj(name, seL4_EndpointObject, label=endpoint_owner) -*/
  /*- else -*/
      /*- set object, endpoint_owner = maybe_object -*/
  /*- endif -*/

  /*# Put it back into the stash #*/
  /*- do _stash(stash_name, (object, endpoint_owner)) -*/

  /*# Create the badged endpoint #*/
  /*- if not is_reader -*/
    /*- set badge = badges[other_index] -*/
    /*- set endpoint = alloc_cap('%s_%s_%d_endpoint_object_cap' % (name, badge, is_reader), object, read=is_reader, write=not is_reader, grantreply=not is_reader) -*/
    /*- do cap_space.cnode[endpoint].set_badge(badge) -*/
  /*- else -*/
      /*- set endpoint = alloc_cap('%s_%d_endpoint_object_cap' % (name, is_reader), object, read=is_reader, write=not is_reader, grantreply=not is_reader) -*/
  /*- endif -*/
  /*- do stash('endpoint', endpoint) -*/
  /*- do stash('badge', badge) -*/
  /*- do stash('badges', badges) -*/
/*- endmacro -*/

/*- macro allocate_rpc_cap_instance(instance) -*/

  /*- set name = '%s_global_rpc_endpoint' % instance.name -*/
  /*- set stash_name = "%s_global_rpc_endpoint" % (name) -*/

  /*# Check the global stash for our endpoint #*/
  /*- set maybe_object = _pop(stash_name) -*/

  /*# Create the endpoint if we need to #*/
  /*- if maybe_object is none -*/
          /*- set endpoint_owner = instance.name -*/
          /*- set object = alloc_obj(name, seL4_EndpointObject, label=endpoint_owner) -*/
  /*- else -*/
      /*- set object, endpoint_owner = maybe_object -*/
  /*- endif -*/

  /*# Put it back into the stash #*/
  /*- do _stash(stash_name, (object, endpoint_owner)) -*/

  /*- set endpoint = alloc_cap('%s_read_endpoint_object_cap' % (name), object, read=True) -*/
  /*- do stash('endpoint', endpoint) -*/

/*- endmacro -*/
