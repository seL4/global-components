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

#define MAGIC_CYCLES 150

struct {
    uint32_t head;
    uint32_t tail;
    char buf[0x1000];
} extern volatile *serial_getchar_buf;

volatile bool flag = 0;


void pre_init(void)
{
    sel4bench_init();
    int err = bench_to_reg_callback(&count_idle, NULL);
    if (err) {
        ZF_LOGE("Failed to register callback handler");
    }
    bench_from_emit();

    set_putchar(serial_putchar_putchar);
}

uint64_t ccount = 0;
uint64_t prev, start, ts, overflows;
uint64_t idle_ccount_start;
uint64_t idle_overflow_start;

void getchar_handler(void)
{
    seL4_Word badge;
    uint64_t total, idle;
    while (1) {
        seL4_Wait(serial_getchar_notification(), &badge);
        char ch = serial_getchar_buf->buf[serial_getchar_buf->head];
        serial_getchar_buf->head = (serial_getchar_buf->head + 1) % sizeof(serial_getchar_buf->buf);
        switch (ch) {
        case 'a':
            idle_start();
            break;
        case 'b':
            idle_stop(&total, &idle);
            printf("idle/tot: %llu/%llu, idle proportion: %f%%\n", idle, total, ((double) idle / (double) total) * 100);
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
        start = (uint64_t)sel4bench_get_cycle_count();
        idle_ccount_start = ccount;
        idle_overflow_start = overflows;

    }
}


void idle_stop(uint64_t *total_ret, uint64_t *idle_ret)
{
    flag = 0;
    uint64_t total = ((uint64_t)sel4bench_get_cycle_count()) - start;
    if ((overflows - idle_overflow_start) > 0) {
        ZF_LOGE("Cycle counter overflowed during measurement. Stats may not be accurate");
    }
    total += ULONG_MAX * (overflows - idle_overflow_start);
    uint64_t idle_total = ccount - idle_ccount_start;
    *total_ret = total;
    *idle_ret = idle_total;
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

int run(void)
{
    getchar_handler();

    return 0;
}
