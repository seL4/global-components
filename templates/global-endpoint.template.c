/*#
 *# Copyright 2014, NICTA
 *#
 *# This software may be distributed and modified according to the terms of
 *# the BSD 2-Clause license. Note that NO WARRANTY is provided.
 *# See "LICENSE_BSD2.txt" for details.
 *#
 *# @TAG(NICTA_BSD)
 #*/

/*? assert(isinstance(instance, str)) ?*/
/*? assert(isinstance(interface, str)) ?*/
/*? assert(isinstance(is_reader, bool)) ?*/

/*- set name = configuration[instance].get('%s_global_endpoint' % interface) -*/
/*- if name is none -*/
  /*? raise(Exception('%s.%s_global_endpoint not set' % (instance, interface))) ?*/
/*- else -*/
  /*- set name = name.strip('"') -*/
/*- endif -*/

/*- set badge = configuration[instance].get('%s_badge' % interface, '"0"').strip('"') -*/

/*- set stash_name = "%s_global_aep" % (name) -*/

/*# Check the global stash for our endpoint #*/
/*- set maybe_aep = _pop(stash_name) -*/
/*- set _aep_object = [] -*/

/*# Create the endpoint if we need to #*/
/*- if maybe_aep is none -*/
    /*- set aep_object = alloc_obj(name, seL4_NotificationObject) -*/
    /*- do _aep_object.append(aep_object) -*/
/*- else -*/
    /*- do _aep_object.append(maybe_aep) -*/
/*- endif -*/

/*- set aep_object = _aep_object.pop() -*/

/*# Put it back into the stash #*/
/*- do _stash(stash_name, aep_object) -*/

/*# Create the badged endpoint #*/
/*- set aep = alloc_cap('%s_%s_aep_object_cap' % (name, badge), aep_object, read=is_reader, write=True) -*/
/*- do cap_space.cnode[aep].set_badge(int(badge, 10)) -*/

/*- do stash('aep', aep) -*/
