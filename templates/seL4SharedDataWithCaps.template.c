/*#
 *#Copyright 2017, Data61, CSIRO (ABN 41 687 119 230)
 *#
 *#SPDX-License-Identifier: BSD-2-Clause
  #*/

#include <camkes/dataport.h>
#include <stdint.h>
#include <stdlib.h>
#include <utils/util.h>
#include <sel4/sel4.h>
#include <camkes/dataport_caps.h>

/*? macros.show_includes(me.instance.type.includes) ?*/

/*- if me in me.parent.from_ends -*/
  /*- set index = me.parent.from_ends.index(me) -*/
  /*- set end = 'from' -*/
/*- elif me in me.parent.to_ends -*/
  /*- set index = me.parent.to_ends.index(me) -*/
  /*- set end = 'to' -*/
/*- endif -*/


/*- set dataport_symbol_name = "%s_%d_%s_data" % (end, index, me.interface.name) -*/
/*- set type_size = macros.dataport_size(me.interface.type) -*/
/*# Get the dataport size from the .size property on the dataport connection. For backwards compatiblity reasons
  # also check the ${interface_name}_size property on the `to` end of the connector.
  #*/
/*- if 'size' in configuration[me.parent.name] -*/
  /*- set dataport_size = configuration[me.parent.name].get('size') -*/
/*- else -*/
  /*- set dataport_size = none -*/
/*- endif -*/
/*- if len(me.parent.to_ends) == 1 -*/
  /*- if '%s_size' % me.parent.to_interface.name in configuration[me.parent.to_instance.name] -*/
    /*- if dataport_size != none -*/
      /*? raise(TemplateError('Duplicate setting for %s dataport size: Cannot set both %s.%s and %s.%s' % (me.parent.name, me.parent.name, 'size', me.parent.to_instance.name, '%s_size' % me.parent.to_interface.name))) ?*/
    /*- endif -*/
    /*- set dataport_size = configuration[me.parent.to_instance.name].get('%s_size' % me.parent.to_interface.name) -*/
  /*- endif -*/
/*- endif -*/
/*- if dataport_size == none -*/
  /*- set dataport_size = 4096 -*/
/*- endif -*/
/*- set page_size = macros.get_page_size(dataport_size, options.architecture) -*/
/*- if page_size == 0 -*/
  /*? raise(TemplateError('Setting %s.%s_size does not meet minimum size requirements. %d must be at least %d and %d aligned' % (me.instance.name, me.interface.name, size, 4096, 4096))) ?*/
/*- endif -*/

/*- set shmem_symbol_size = "MAX_UNSAFE(%s, %s)" % (type_size, dataport_size) -*/
/*? macros.shared_buffer_symbol(sym=dataport_symbol_name, shmem_size=shmem_symbol_size, page_size=page_size) ?*/

/*- if options.architecture in ['aarch32', 'arm_hyp', 'aarch64', 'ia32', 'x86_64'] -*/
    /*- set cached_default = true -*/
/*- else -*/
    /*- set cached_default = false -*/
/*- endif -*/
/*- set cached = configuration[me.instance.name].get('%s_hardware_cached' % me.interface.name, cached_default) -*/

/*- if 'paddr' in configuration[me.parent.name] and me in me.parent.to_ends -*/
  /*- set paddr = configuration[me.parent.name].get('paddr') -*/
/*- else -*/
  /*- set paddr = None -*/
/*- endif -*/

/*- set perm = macros.get_perm(configuration, me.instance.name, me.interface.name) -*/
/*? register_shared_variable('%s_data' % me.parent.name, dataport_symbol_name, dataport_size, frame_size=page_size, perm=perm, cached=cached, paddr=paddr) ?*/

volatile /*? macros.dataport_type(me.interface.type) ?*/ * /*? me.interface.name ?*/ =
    (volatile /*? macros.dataport_type(me.interface.type) ?*/ *) &/*? dataport_symbol_name ?*/;

/*- set id = composition.connections.index(me.parent) -*/

int /*? me.interface.name ?*/_wrap_ptr(dataport_ptr_t *p, void *ptr) {
    if ((uintptr_t)ptr < (uintptr_t)/*? me.interface.name ?*/ ||
            (uintptr_t)ptr >= (uintptr_t)/*? me.interface.name ?*/ + /*? macros.dataport_size(me.interface.type) ?*/) {
        return -1;
    }
    p->id = /*? id ?*/;
    p->offset =  (off_t)((uintptr_t)ptr - (uintptr_t)/*? me.interface.name ?*/);
    return 0;
}

void * /*? me.interface.name ?*/_unwrap_ptr(dataport_ptr_t *p) {
    if (p->id == /*? id ?*/) {
        return (void*)((uintptr_t)/*? me.interface.name ?*/ + (uintptr_t)p->offset);
    } else {
        return NULL;
    }
}

/*- set read_perm = False -*/
/*- set write_perm = False -*/
/*- if 'R' in perm -*/
    /*- set read_perm = True -*/
/*- endif -*/
/*- if 'W' in perm -*/
    /*- set write_perm = True -*/
/*- endif -*/

/*- set frame_objs = get_shared_variable_backing_frames('%s_data' % me.parent.name, dataport_size) -*/
/*- set frame_caps = [] -*/
static seL4_CPtr frame_caps[] = {
/*- for (i, frame) in enumerate(frame_objs) -*/
    /*- set frame_cap = alloc_cap('%s_%d' % ('%s_data' % me.parent.name, i), frame, read=read_perm, write=write_perm) -*/
    /*- do frame_caps.append(frame_cap) -*/
    /*? frame_cap ?*/,
/*- endfor -*/
};
/*- set num_frame_caps = len(frame_caps) -*/


static seL4_CPtr /*? me.interface.name ?*/_get_nth_frame_cap(unsigned int n) {
    return frame_caps[n];
}

static seL4_CPtr* /*? me.interface.name ?*/_get_frame_caps(void) {
    return frame_caps;
}

static unsigned int /*? me.interface.name ?*/_get_num_frame_caps(void) {
    return /*? num_frame_caps ?*/;
}

static unsigned int /*? me.interface.name ?*/_get_id(void) {
    return /*? configuration[me.instance.name].get('%s_id' % me.interface.name, 0) ?*/;
}

static size_t /*? me.interface.name ?*/_get_size(void) {
    return /*? dataport_size ?*/;
}

static seL4_CapRights_t /*? me.interface.name ?*/_get_rights(void) {
    /*- if perm is none -*/
        return seL4_AllRights;
    /*- else -*/
        /*- set read = int(read_perm) -*/
        /*- set write = int(write_perm) -*/
        /*# The 0 grant/grantreply argument below is because the grant operations are meaningless on dataports.
         *# The lack of execute permission is because cap rights don't capture the right to execute
         *# the contents of a mapped-in page. #*/
        return seL4_CapRights_new(0, 0, /*? read ?*/, /*? write ?*/);
    /*- endif -*/
}

dataport_caps_handle_t /*? me.interface.name ?*/_handle = {
    .get_nth_frame_cap = /*? me.interface.name ?*/_get_nth_frame_cap,
    .get_id = /*? me.interface.name ?*/_get_id,
    .get_num_frame_caps = /*? me.interface.name ?*/_get_num_frame_caps,
    .get_frame_caps = /*? me.interface.name ?*/_get_frame_caps,
    .get_size = /*? me.interface.name ?*/_get_size,
    .get_rights = /*? me.interface.name ?*/_get_rights,
};
