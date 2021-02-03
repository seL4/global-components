/*#
 *#Copyright 2017, Data61, CSIRO (ABN 41 687 119 230)
 *#
 *#SPDX-License-Identifier: BSD-2-Clause
  #*/

#include <camkes/dataport.h>
#include <sel4/sel4.h>
#include <utils/util.h>

/*? macros.show_includes(me.instance.type.includes) ?*/

/*# Assign client ids and badges #*/
/*- set badges = namespace() -*/
/*- if client_ids is not undefined -*/
    /*- set badges.badges = client_ids.badges -*/
/*- else -*/
    /*- from 'rpc-connector.c' import allocate_badges with context -*/
    /*- do allocate_badges(badges) -*/
/*- endif -*/
/*- set badges = badges.badges -*/

/*# Enumerate all the incoming interfaces #*/
/*- set shmems = [] -*/
/*- set client_ids = set() -*/
/*- for c in me.parent.from_ends -*/

    /*- set client_id = badges[loop.index0] -*/

    /*- if client_id not in client_ids -*/
        /*- do client_ids.add(client_id) -*/

        /*- set shmem_size = configuration[c.instance.name].get("%s_shmem_size" % c.interface.name, 4096) -*/
        /*- set shmem_section = 'to_%s_%s' % (me.interface.name, client_id) -*/
        /*- set shmem_symbol = 'to_%s_%s_data' % (me.interface.name, client_id) -*/
        /*- set shmem_name = "%s_buf_%s" % (me.interface.name, client_id) -*/
        /*- set page_size = macros.get_page_size(shmem_size, options.architecture) -*/
        /*- if page_size == 0 -*/
          /*? raise(TemplateError('Setting %s.%s_shmem_size does not meet minimum size and alignment requirements. %d must be at least %d and %d aligned' % (c.instance.name, c.interface.name, size, 4096, 4096))) ?*/
        /*- endif -*/
        /*- set page_size_bits = int(math.log(page_size, 2)) -*/

        /*? macros.shared_buffer_symbol(sym=shmem_symbol, shmem_size=shmem_size, page_size=page_size) ?*/
        /*? register_shared_variable('%s_%s_data' % (me.parent.name, client_id), shmem_symbol, shmem_size, frame_size=page_size) ?*/

        volatile void * /*? shmem_name ?*/ = (volatile void *) & /*? shmem_symbol ?*/;

        /*- do shmems.append((shmem_name, client_id, shmem_size)) -*/
    /*- else -*/
        /* skipping /*? client_id ?*/ */
    /*- endif -*/

/*- endfor -*/


void * /*? me.interface.name ?*/_buf(seL4_Word client_id) {
    /*- if len(shmems) == 0 -*/
        return NULL;
    /*- else -*/
        switch (client_id) {
            /*- for symbol, id, _ in shmems -*/
            case /*? id ?*/:
                return (void *) /*? symbol ?*/;
            /*- endfor -*/
        default:
            return NULL;
        }
    /*- endif -*/
}

size_t /*? me.interface.name ?*/_buf_size(seL4_Word client_id) {
    /*- if len(shmems) == 0 -*/
        return 0;
    /*- else -*/
        switch (client_id) {
            /*- for _, id, size in shmems -*/
            case /*? id ?*/:
                return ROUND_UP_UNSAFE(/*? size ?*/, PAGE_SIZE_4K);
            /*- endfor -*/
        default:
            return 0;
        }
    /*- endif -*/
}

unsigned int /*? me.interface.name ?*/_num_badges(void) {
    return /*? len(shmems) ?*/;
}

seL4_Word /*? me.interface.name ?*/_enumerate_badge(unsigned int i) {
    /*- if len(shmems) == 0 -*/
       return -1;
    /*- else -*/
        switch (i) {
            /*- for _, id, _ in shmems -*/
                case /*? loop.index0 ?*/:
                    return /*? id ?*/;
            /*- endfor -*/
        default:
            return -1;
        }
    /*- endif -*/
}


int /*? me.interface.name ?*/_wrap_ptr(dataport_ptr_t *p, void *ptr) {
    return -1;
}

void * /*? me.interface.name ?*/_unwrap_ptr(dataport_ptr_t *p) {
    return NULL;
}
