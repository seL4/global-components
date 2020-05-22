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


uint64_t ccount = 0, tot = 0;
uint64_t prev, start, ts;
uint64_t idle_ccount_start;

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
            break;
        default:
            break;
        }
    }
}


void idle_start(void) {
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

    }
}


void idle_stop(uint64_t *total_ret, uint64_t *kernel_ret, uint64_t *idle_ret) {
    flag = 0;
    uint64_t total = ((uint64_t)sel4bench_get_cycle_count()) - start;
    uint64_t idle_total = ccount - idle_ccount_start;
#ifdef CONFIG_BENCHMARK_TRACK_UTILISATION
    seL4_BenchmarkFinalizeLog();

    seL4_BenchmarkGetThreadUtilisation(camkes_get_tls()->tcb_cap);
    uint64_t *buffer = (uint64_t*)&seL4_GetIPCBuffer()->msg[0];
    printf("total,%"PRIu64"\nidle,%"PRIu64"\nkernel,%"PRIu64"\n", buffer[BENCHMARK_TOTAL_UTILISATION], buffer[BENCHMARK_IDLE_LOCALCPU_UTILISATION], buffer[BENCHMARK_KERNEL_UTILISATION]);
    seL4_BenchmarkDumpAllThreadsUtilisation();
#endif
    trace_stop_emit();
    printf("idle/tot: %llu/%llu, idle proportion: %f%%\n", idle_total, total, ((double) idle_total / (double) total) * 100);
    *total_ret = total;
    *idle_ret = idle_total;
#ifdef CONFIG_BENCHMARK_TRACK_UTILISATION
    *kernel_ret = buffer[BENCHMARK_KERNEL_UTILISATION];
#endif
}


void count_idle(UNUSED void *arg)
{
    prev = sel4bench_get_cycle_count();
    ccount = 0;

    while (1) {
        ts = (uint64_t)sel4bench_get_cycle_count();
        uint64_t diff;

        /* handle overflow */
        if (ts < prev) {
            printf("Overflow\n");
            diff = ULONG_MAX - prev + ts + 1;
            start = ts;
            tot += ULONG_MAX + ts + 1;
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
