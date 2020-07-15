/*
 * Copyright 2020, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */

#define _VAR_STRINGIZE(...) #__VA_ARGS__
#define VAR_STRINGIZE(...) _VAR_STRINGIZE(__VA_ARGS__)

#include <camkes-BenchUtiliz.h>;


#define single_threaded_component() \
    control; \
    attribute int single_threaded = 1; \
    BenchUtiliz_trace_interfaces_maybe(trace) \
    attribute int number_tracepoints = 10; \
    attribute int enable_tracing = 0;
