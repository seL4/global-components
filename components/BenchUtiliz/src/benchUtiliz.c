/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <limits.h>
#include <autoconf.h>
#include <camkes.h>
#include <stdio.h>
#include <sel4bench/sel4bench.h>
#include <camkes/tls.h>
#include <sel4/benchmark_utilisation_types.h>

#define MAGIC_CYCLES 150

struct {
    uint32_t head;
    uint32_t tail;
    char buf[0x1000];
} extern WEAK volatile *serial_getchar_buf;
WEAK seL4_CPtr serial_getchar_notification(void);
volatile bool flag = 0;

uint64_t ccount = 0;
uint64_t prev, start, ts, overflows;
uint64_t idle_ccount_start;
uint64_t idle_overflow_start;

void getchar_handler(void)
{
    seL4_Word badge;
    uint64_t total, kernel, idle;
    while (1) {
        seL4_Wait(serial_getchar_notification(), &badge);
        char ch = serial_getchar_buf->buf[serial_getchar_buf->head];
        serial_getchar_buf->head = (serial_getchar_buf->head + 1) % sizeof(serial_getchar_buf->buf);
        switch (ch) {
        case 'a':
            idle_start();
            break;
        case 'b':
            idle_stop(&total, &kernel, &idle);
            printf("{\n");
            printf("\"total\": %"PRIu64",\n", total);
            printf("\"kernel\": %"PRIu64",\n", kernel);
            printf("\"idle\": %"PRIu64"\n", idle);
            printf("}\n", idle);
            break;
        default:
            break;
        }
    }
}


void idle_start(void)
{
    if (!flag) {
        flag = 1;
        printf("Measurment starting...\n");
        trace_start_emit();
#ifdef CONFIG_BENCHMARK_TRACK_UTILISATION
        seL4_BenchmarkResetAllThreadsUtilisation();
        seL4_BenchmarkResetLog();
#endif

        start = (uint64_t)sel4bench_get_cycle_count();
        idle_ccount_start = ccount;
        idle_overflow_start = overflows;

    }
}


void idle_stop(uint64_t *total_ret, uint64_t *kernel_ret, uint64_t *idle_ret)
{
    flag = 0;
    uint64_t total = ((uint64_t)sel4bench_get_cycle_count()) - start;
    if ((overflows - idle_overflow_start) > 0) {
        ZF_LOGE("Cycle counter overflowed during measurement. Stats may not be accurate");
    }
    total += ULONG_MAX * (overflows - idle_overflow_start);
    uint64_t idle_total = ccount - idle_ccount_start;
#ifdef CONFIG_BENCHMARK_TRACK_UTILISATION
    seL4_BenchmarkFinalizeLog();

    seL4_BenchmarkGetThreadUtilisation(camkes_get_tls()->tcb_cap);
    uint64_t *buffer = (uint64_t *)&seL4_GetIPCBuffer()->msg[0];
    seL4_BenchmarkDumpAllThreadsUtilisation();
    *kernel_ret = buffer[BENCHMARK_TOTAL_KERNEL_UTILISATION];
#else
    *kernel_ret = 0;
#endif
    *total_ret = total;
    *idle_ret = idle_total;
    trace_stop_emit();
}


void count_idle(UNUSED void *arg)
{
    prev = sel4bench_get_cycle_count();
    ccount = 0;
    overflows = 0;

    while (1) {
        ts = (uint64_t)sel4bench_get_cycle_count();
        uint64_t diff;

        /* Handle overflow: This thread needs to run at least 2 times
           within any ULONG_MAX cycles period to detect overflows.
           This also assumes that the cycle counter overflows every ULONG_MAX
           cycles which may not be true for all platforms.*/
        if (ts < prev) {
            diff = ULONG_MAX - prev + ts + 1;
            overflows++;
        } else {
            diff = ts - prev;
        }

        if (diff < MAGIC_CYCLES) {
            COMPILER_MEMORY_FENCE();
            ccount += diff;
            COMPILER_MEMORY_FENCE();
        }
        prev = ts;
    }
}

void pre_init(void)
{
    sel4bench_init();
    int err = bench_to_reg_callback(&count_idle, NULL);
    if (err) {
        ZF_LOGE("Failed to register callback handler");
    }
    bench_from_emit();

    if (serial_putchar_putchar) {
        set_putchar(serial_putchar_putchar);
    }
}

int run(void)
{
    if (&serial_getchar_buf) {
        getchar_handler();
    }

    return 0;
}
