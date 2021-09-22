/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <assert.h>
#include <camkes/dataport.h>
#include <camkes/irq.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <platsupport/io.h>
#include <platsupport/irq.h>
#include <utils/util.h>
#include <sel4/sel4.h>

/*- import 'dtb-query-common.template.c' as dtb_macros with context -*/

/*# Grab the DTB object made from the previous stages of the parsing #*/
/*- set configuration_name = '%s.%s' % (me.instance.name, me.interface.name) -*/
/*- set dtb_config = configuration[configuration_name].get('dtb') -*/
/*- if dtb_config is none -*/
    /*# DTB query hasn't been made, raise a fault #*/
    /*? raise(TemplateError('Missing DTB attribute, create a DTB query to attribute %s.dtb.' % (me.interface.name))) ?*/
/*- endif -*/
/*- set dtb_query = dtb_config.get('query')  -*/
/*- if dtb_query is none -*/
    /*# DTB path hasn't been passed, raise a fault #*/
    /*? raise(TemplateError('Missing DTB query, assign a DTB path to attribute %s.dtb.' % (me.interface.name))) ?*/
/*- endif -*/
/*- if len(dtb_query) != 1 -*/
    /*? raise(TemplateError('Invalid number of DTB paths, assign a single DTB path to attribute %s.dtb.' % (me.interface.name))) ?*/
/*- endif -*/
/*- if dtb_query[0] is none -*/
    /*? raise(TemplateError('Missing DTB path, assign a single DTB path to attribute %s.dtb.' % (me.interface.name))) ?*/
/*- endif -*/
/*- set dtb = dtb_query[0]  -*/

/*# Check if we want interrupts #*/
/*- set generate_interrupts = configuration[configuration_name].get('generate_interrupts', none) -*/

/*# ================ #*/
/*# Register section #*/
/*# ================ #*/

/*? dtb_macros.parse_dtb_node_reg(dtb) ?*/
/*- set reg_set = pop('reg_set') -*/
/*- set cached = configuration[configuration_name].get('hardware_cached', False) -*/

/*- for paddr, size in reg_set -*/

        /*# Get the next multiple of 4K that can fit the register #*/
        /*- set size = macros.next_page_multiple(size, options.architecture) -*/
        /*- set page_size = macros.get_page_size(size, options.architecture) -*/
        /*- set page_size_bits = int(math.log(page_size, 2)) -*/

        /*- set dataport_symbol_name = "from_%d_%s_data" % (loop.index0, me.interface.name) -*/
        struct {
            char content[ROUND_UP_UNSAFE(/*? size ?*/,
                SIZE_BITS_TO_BYTES(/*? page_size_bits ?*/))];
        } /*? dataport_symbol_name ?*/
                ALIGN(/*? page_size ?*/)
                SECTION("align_/*? page_size_bits ?*/bit")
                VISIBLE
                USED;

        /*- set reg_interface_name = '%s_%d' % (me.interface.name, loop.index0) -*/

        /*- set frame_caps = [] -*/
        /*? register_shared_variable('%s_data' % reg_interface_name, dataport_symbol_name, size, frame_size=page_size, perm='RW', paddr=paddr, cached=cached, with_mapping_caps=frame_caps) ?*/

        /*# Assign a name for this particular set of registers #*/
        volatile void * /*? reg_interface_name ?*/ =
            (volatile void *) & /*? dataport_symbol_name ?*/;

        /*- set id = composition.connections.index(me.parent) -*/

        __attribute__((used)) __attribute__((section("_dataport_frames")))
        dataport_frame_t /*? reg_interface_name ?*/_frames[] = {

        /*- for cap in frame_caps -*/
            {
                .paddr = /*? paddr + loop.index0 * page_size ?*/,
                .cap = /*? cap ?*/,
                .size = /*? page_size ?*/,
                .vaddr = (uintptr_t)&(/*? dataport_symbol_name ?*/.content[/*? loop.index0 * page_size ?*/]),
            },
        /*- endfor -*/
        };
        /*# We only pull frame_caps from the stash. This is because only one caller of register_shared_variable
            should pass a frames parameter. By not stashing the frame_objs we ensure that only the original
            creator passed the frames, and everyone else will still have a None here #*/


        /* Flush data corresponding to the dataport-relative address range from the CPU cache */
        int /*? reg_interface_name ?*/_flush_cache(size_t start_offset UNUSED, size_t size UNUSED, dma_cache_op_t cache_op UNUSED) {
            return camkes_dataport_flush_cache(start_offset, size,
                                               (uintptr_t) &/*? dataport_symbol_name ?*/.content,
                                               /*? size ?*/, cache_op);
        }

/*- endfor -*/

/*# ================== #*/
/*# Interrupts section #*/
/*# ================== #*/

/*- if generate_interrupts -*/

    /*- set ntfn_obj = alloc_obj('%s_ntfn' % me.interface.name, seL4_NotificationObject) -*/
    /*- set root_ntfn = alloc_cap('%s_root_ntfn' % me.interface.name, ntfn_obj, read=True, write=True) -*/

    /*- set irq_handler_pairs = [] -*/

    /*- set interrupt_struct_prefix = '%s_irq' % (me.interface.name) -*/

    /*# CAmkES has a maximum limit of 28 bits for badges, #*/
    /*# highly unlikely a device has greater than 28 #*/
    /*? dtb_macros.parse_dtb_node_interrupts(dtb, 28, options.architecture) ?*/
    /*- set badges =  macros.global_endpoint_badges(composition, me, configuration, options.architecture) -*/
    /*- set irq_set = pop('irq_set') -*/
    /*- set name = '%s_global_endpoint' % me.instance.name -*/
/*- set stash_name = "%s_global_notification" % (name) -*/

  /*# Check the global stash for our endpoint #*/
  /*- set maybe_notification = _pop(stash_name) -*/

  /*# Create the endpoint if we need to #*/
  /*- if maybe_notification is none -*/
          /*- set notification_owner = me.instance.name -*/
          /*- set notification_object = alloc_obj(name, seL4_NotificationObject, label=notification_owner) -*/
  /*- else -*/
      /*- set notification_object, notification_owner = maybe_notification -*/
  /*- endif -*/

  /*# Put it back into the stash #*/
  /*- do _stash(stash_name, (notification_object, notification_owner)) -*/

  /*# Create the badged endpoint #*/

    /*- for i,irq_node in enumerate(irq_set) -*/
        /*- set _irq = irq_node['irq'] -*/

        /*- set interrupt_ntfn = alloc_cap('%s_%s_%d_notification_object_cap' % (name, badges[i], False), notification_object, read=False, write=True) -*/
        /*- do cap_space.cnode[interrupt_ntfn].set_badge(badges[i]) -*/

        /*- set irq = alloc('%s_irq_%d' % (me.interface.name, i), seL4_IRQHandler, number=_irq, notification=cap_space.cnode[interrupt_ntfn]) -*/

        /*# Add the interrupt number to the IRQ num list for later #*/
        /*- do irq_handler_pairs.append((_irq, irq)) -*/

        /*- set interrupt_interface_name = '%s_%d' % (me.interface.name, i) -*/

        /*# Add an entry to the allocated_irqs ELF section #*/
        static allocated_irq_t /*? interrupt_struct_prefix ?*/_/*? i ?*/ = {
            .irq_handler = /*? irq ?*/,
            .irq = { .type = PS_INTERRUPT, .irq = { .number = /*? _irq ?*/ }},
            .is_allocated = false,
            .callback_fn = NULL,
            .callback_data = NULL
        };
        USED SECTION("_allocated_irqs")
        allocated_irq_t * /*? interrupt_struct_prefix ?*/_/*? i ?*/_ptr = &/*? interrupt_struct_prefix ?*/_/*? i ?*/;

        static int /*? me.interface.name ?*/_acknowledge_cb_/*? i ?*/(UNUSED void* cookie) {
            return seL4_IRQHandler_Ack(/*? irq ?*/);
        }

        /*# Add an entry to the global_notification_irqs ELF section #*/
        /*- set irq_handler_struct_name = '%s_irq_handler_%d' % (me.interface.name, i) -*/
        static global_notification_irq_handler_t /*? irq_handler_struct_name ?*/ = {
            .badge = /*? badges[i] ?*/,
            .ack_fun = /*? me.interface.name ?*/_acknowledge_cb_/*? i ?*/,
            .allocated_ref = &/*? interrupt_struct_prefix ?*/_/*? i ?*/,
        };

        USED SECTION("_global_notification_irqs")
        global_notification_irq_handler_t * /*? irq_handler_struct_name ?*/_ptr = &/*? irq_handler_struct_name ?*/;

    /*- endfor -*/

/*- endif -*/
