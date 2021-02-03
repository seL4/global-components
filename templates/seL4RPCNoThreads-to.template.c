/*
 * Copyright 2020, Data61, ABN 41 687 119 230.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


/*- from 'rpc-connector.c' import establish_recv_rpc, recv_first_rpc, complete_recv, begin_recv, begin_reply, complete_reply, reply_recv with context -*/
/*- from 'global-endpoint.template.c' import allocate_rpc_endpoint_cap with context -*/
/*- import 'helpers/error.c' as error with context -*/
/*- import 'helpers/marshal.c' as marshal with context -*/

#include <autoconf.h>
#include <sel4camkes/gen_config.h>
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <camkes/error.h>
#include <camkes/marshal_macros.h>
#include <camkes/tls.h>
#include <sel4/sel4.h>
#include <camkes/dataport.h>
#include <utils/util.h>

/*? macros.show_includes(me.instance.type.includes) ?*/
/*? macros.show_includes(me.interface.type.includes) ?*/

/*- set connector = namespace() -*/

/*- set connector.language = 'c' -*/
/*# Establish the buffer for message contents #*/
/*- set connector.userspace_ipc = False -*/
/*- set base = '((void*)&seL4_GetIPCBuffer()->msg[0])' -*/
/*- set buffer_size = '(seL4_MsgMaxLength * sizeof(seL4_Word))' -*/
/*- set connector.lock = False -*/
/*- set connector.send_buffer = base -*/
/*- set connector.recv_buffer = base -*/
/*- set connector.send_buffer_size = buffer_size -*/
/*- set connector.recv_buffer_size = buffer_size -*/
/*- set connector.recv_buffer_size_fixed = connector.userspace_ipc -*/

/*# Ensure the endpoint is allocated #*/
/*- do allocate_rpc_endpoint_cap(me, is_reader=True, other_end=me.parent.from_ends[0]) -*/
/*- set connector.badges = pop('badges') -*/
/*- set connector.ep = pop('endpoint') -*/
/*- set connector.badge_symbol = '%s_badge' % me.interface.name -*/
#include <sel4/sel4.h>
static seL4_Word /*? connector.badge_symbol ?*/ = 0;

seL4_Word /*? me.interface.name ?*/_get_sender_id(void) {
    switch (/*? connector.badge_symbol ?*/) {
    /*- for from_end in me.parent.from_ends -*/
        case /*? connector.badges[loop.index0] ?*/:
			return /*? loop.index0 ?*/;
			break;
    /*- endfor -*/
		default:
			ZF_LOGE("Invalid Badge: %"PRIdPTR" doesn't match any client ID", /*? connector.badge_symbol ?*/);
	}

    return -1;
}


/*- set instance = me.instance.name -*/
/*- set interface = me.interface.name -*/

/* Interface-specific error handling */
/*- set error_handler = '%s_error_handler' % me.interface.name -*/
/*? error.make_error_handler(interface, error_handler) ?*/

#define CAMKES_INTERFACE_NAME "/*? interface ?*/"
#define CAMKES_INSTANCE_NAME "/*? instance ?*/"
#define CAMKES_ERROR_HANDLER /*? error_handler ?*/

/*# Construct a dict from interface types to list of from ends indecies #*/
/*- set type_dict = {} -*/
/*- for f in me.parent.from_ends -*/
    /*- set cur_list = type_dict.get(f.interface.type, []) -*/
    /*- do cur_list.append(loop.index0) -*/
    /*- do type_dict.update({f.interface.type: cur_list}) -*/
/*- endfor -*/

/*- for j, from_type in enumerate(type_dict.keys()) -*/
    /*-- set methods_len = len(from_type.methods) -*/
    /*-- for m in from_type.methods -*/
        extern /*- if m.return_type is not none --*/
            /*? macros.show_type(m.return_type) ?*/ /*- else --*/
            void /*- endif --*/
            /*?- me.interface.name ?*/_/*? m.name ?*/(
                /*?- marshal.show_input_parameter_list(m.parameters, ['in', 'refin', 'out', 'inout']) ?*/
                /*-- if len(m.parameters) == 0 -*/
                    void
                /*-- endif --*/
            );

        /*- set input_parameters = list(filter(lambda('x: x.direction in [\'refin\', \'in\', \'inout\']'), m.parameters)) -*/
        /*? marshal.make_unmarshal_input_symbols(m.name, '%s_unmarshal_inputs' % m.name, methods_len, input_parameters, connector.recv_buffer_size_fixed) ?*/

        /*- set output_parameters = list(filter(lambda('x: x.direction in [\'out\', \'inout\']'), m.parameters)) -*/
        /*? marshal.make_marshal_output_symbols(m.name, '%s_marshal_outputs' % m.name, output_parameters, m.return_type) ?*/

    /*- endfor -*/
/*- endfor -*/


int /*? me.interface.name ?*/_handle_message(seL4_MessageInfo_t *info, seL4_Word badge)
{

    unsigned size = seL4_MessageInfo_get_length(*info) * sizeof(seL4_Word);
    assert(size <= seL4_MsgMaxLength * sizeof(seL4_Word));
    unsigned length = 0;
    /*? connector.badge_symbol ?*/ = badge;
        /*- if len(type_dict.keys()) > 1 -*/
            switch (/*? connector.badge_symbol ?*/) {
        /*- endif -*/
        /*- for from_index, from_type in enumerate(type_dict.keys()) -*/
            /*- set methods_len = len(from_type.methods) -*/
            /*- if len(type_dict.keys()) > 1 -*/
                /*- for from_index in type_dict.get(from_type) -*/
                    case /*? connector.badges[from_index] ?*/: {
                /*- endfor -*/
            /*- endif -*/

            /*- if methods_len <= 1 -*/
                unsigned call = 0;
                unsigned * call_ptr = &call;
            /*- else -*/
                /*- set type = macros.type_to_fit_integer(methods_len) -*/
                /*? type ?*/ call;
                /*? type ?*/ * call_ptr = &call;
                unsigned offset = 0;
                UNMARSHAL_PARAM(call_ptr, /*? connector.recv_buffer ?*/, size , offset, "/*? me.interface.name ?*/", "method_index", ({
                        /*? complete_recv(connector) ?*/
                        goto begin_recv;
                }));
            /*- endif -*/

            switch (* call_ptr) {
                /*-- for i, m in enumerate(from_type.methods) -*/
                    case /*? i ?*/: { /*? '%s%s%s%s%s' % ('/', '* ', m.name, ' *', '/') ?*/
                        /*#- Declare parameters. #*/
                        /*-- for p in m.parameters -*/

                            /*-- if p.array -*/
                                size_t p_/*? p.name ?*/_sz;
                                size_t * p_/*? p.name ?*/_sz_ptr = &p_/*? p.name ?*/_sz;
                                /*-- if p.type == 'string' -*/
                                    char ** p_/*? p.name ?*/ = NULL;
                                    char *** p_/*? p.name ?*/_ptr = &p_/*? p.name ?*/;
                                /*-- else -*/
                                    /*? macros.show_type(p.type) ?*/ * p_/*? p.name ?*/ = NULL;
                                    /*? macros.show_type(p.type) ?*/ ** p_/*? p.name ?*/_ptr = &p_/*? p.name ?*/;
                                /*-- endif -*/
                            /*-- elif p.type == 'string' -*/
                                char * p_/*? p.name ?*/ = NULL;
                                char ** p_/*? p.name ?*/_ptr = &p_/*? p.name ?*/;
                            /*-- else -*/
                                /*? macros.show_type(p.type) ?*/ p_/*? p.name ?*/;
                                /*? macros.show_type(p.type) ?*/ * p_/*? p.name ?*/_ptr = &p_/*? p.name ?*/;
                            /*-- endif -*/
                        /*-- endfor -*/

                        /* Unmarshal parameters */
                        /*-- set input_parameters = list(filter(lambda('x: x.direction in [\'refin\', \'in\', \'inout\']'), m.parameters)) -*/
                        int err = /*? marshal.call_unmarshal_input('%s_unmarshal_inputs' % m.name, connector.recv_buffer, "size", input_parameters, namespace_prefix='p_') ?*/;
                        if (unlikely(err != 0)) {
                            /* Error in unmarshalling; return to event loop. */
                            /*?- complete_recv(connector) ?*/
                            goto begin_recv;
                        }
                        /* Call the implementation */
                        /*-- set ret = "%s_ret" % (m.name) -*/
                        /*-- set ret_sz = "%s_ret_sz" % (m.name) -*/
                        /*-- set ret_ptr = "%s_ret_ptr" % (m.name) -*/
                        /*-- set ret_sz_ptr = "%s_ret_sz_ptr" % (m.name) -*/
                        /*-- if m.return_type is not none -*/
                            /*-- if m.return_type == 'string' -*/
                                char * /*? ret ?*/;
                                char ** /*? ret_ptr ?*/ = &/*? ret ?*/;
                            /*-- else -*/
                                /*? macros.show_type(m.return_type) ?*/ /*? ret ?*/;
                                /*? macros.show_type(m.return_type) ?*/ * /*? ret_ptr ?*/ = &/*? ret ?*/;
                            /*-- endif --*/
                            * /*? ret_ptr ?*/ =
                        /*-- endif --*/
                        /*? me.interface.name ?*/_/*? m.name ?*/(
                            /*-- for p in m.parameters -*/
                                /*-- if p.array -*/
                                    /*-- if p.direction == 'in' -*/* /*- endif --*/
                                    p_/*? p.name ?*/_sz_ptr,
                                /*-- endif -*/
                                /*-- if p.direction =='in' --*/* /*- endif --*/
                                p_/*? p.name ?*/_ptr
                                /*-- if not loop.last -*/,/*- endif --*/
                            /*-- endfor --*/
                        );

                        /*? complete_recv(connector) ?*/
                        /*? begin_reply(connector) ?*/

                        /* Marshal the response */
                        /*-- set output_parameters = list(filter(lambda('x: x.direction in [\'out\', \'inout\']'), m.parameters)) -*/
                        length = /*? marshal.call_marshal_output('%s_marshal_outputs' % m.name, connector.send_buffer, connector.send_buffer_size, output_parameters, m.return_type, ret_ptr, namespace_prefix='p_') ?*/;

                        /*#- We no longer need anything we previously malloced #*/
                        /*-- if m.return_type == 'string' -*/
                            free(* /*? ret_ptr ?*/);
                        /*-- endif -*/
                        /*-- for p in m.parameters -*/
                            /*-- if p.array -*/
                                /*-- if p.type == 'string' -*/
                                    for (int mcount = 0; mcount < * p_/*? p.name ?*/_sz_ptr; mcount++) {
                                        free((* p_/*? p.name ?*/_ptr)[mcount]);
                                    }
                                /*-- endif -*/
                                free(* p_/*? p.name ?*/_ptr);
                            /*-- elif p.type == 'string' -*/
                                free(* p_/*? p.name ?*/_ptr);
                            /*-- endif -*/
                        /*-- endfor -*/

                        /* Check if there was an error during marshalling. We do
                         * this after freeing internal parameter variables to avoid
                         * leaking memory on errors.
                         */
                        if (unlikely(length == UINT_MAX)) {
                            /*?- complete_reply(connector) ?*/
                            goto begin_recv;
                        }

                        goto reply_recv;
                    }
                /*- endfor -*/
                default: {
                    ERR(/*? error_handler ?*/, ((camkes_error_t){
                        .type = CE_INVALID_METHOD_INDEX,
                        .instance = "/*? instance ?*/",
                        .interface = "/*? interface ?*/",
                        .description = "invalid method index received in /*? me.interface.name ?*/",
                        .lower_bound = 0,
                        .upper_bound = /*? methods_len ?*/ - 1,
                        .invalid_index = * call_ptr,
                    }), ({
                        /*? complete_recv(connector) ?*/
                        goto begin_recv;
                    }));
                }
            }
            /*- if len(type_dict.keys()) > 1 -*/
                break;
            }
            /*- endif -*/
        /*- endfor -*/
        /*- if len(type_dict.keys()) > 1 -*/
            default:
                ERR(/*? error_handler ?*/, ((camkes_error_t){
                    .type = CE_MALFORMED_RPC_PAYLOAD,
                    .instance = "/*? instance ?*/",
                    .interface = "/*? interface ?*/",
                    .description = "unknown badge while unmarshalling method in /*? me.interface.name ?*/",
                    .length = size,
                    .current_index = /*? connector.badge_symbol ?*/,
                    }), ({
                        /*? complete_recv(connector) ?*/
                        goto begin_recv;
                    }));
                break;
        }
        /*- endif -*/

/* These labels are used to reduce the same code getting generated for each
 * case statement.
 */
reply_recv: {
    *info = seL4_MessageInfo_new(0, 0, 0, ROUND_UP_UNSAFE(length, sizeof(seL4_Word)) / sizeof(seL4_Word));
    return 1;
}

begin_recv: {
	return 0;
}

}
