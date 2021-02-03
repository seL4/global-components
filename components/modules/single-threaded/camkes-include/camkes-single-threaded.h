/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#define _VAR_STRINGIZE(...) #__VA_ARGS__
#define VAR_STRINGIZE(...) _VAR_STRINGIZE(__VA_ARGS__)

#include <camkes-BenchUtiliz.h>


#define single_threaded_component() \
    control; \
    attribute int single_threaded = 1; \
    BenchUtiliz_trace_interfaces_maybe(trace) \
    attribute int number_tracepoints = 10; \
    attribute int enable_tracing = 0;
