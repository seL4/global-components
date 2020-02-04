/*#
 *#Copyright 2017, Data61
 *#Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 *#ABN 41 687 119 230.
 *#
 *#This software may be distributed and modified according to the terms of
 *#the BSD 2-Clause license. Note that NO WARRANTY is provided.
 *#See "LICENSE_BSD2.txt" for details.
 *#
 *#@TAG(DATA61_BSD)
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

  /*- set badge = macros.global_endpoint_badges(composition, interface_end, configuration) -*/
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
