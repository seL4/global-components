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
    set_putchar(serial_putchar_putchar);
}


void getchar_handler(void)
{
    seL4_Word badge;
    while (1) {
        seL4_Wait(serial_getchar_notification(), &badge);
        char ch = serial_getchar_buf->buf[serial_getchar_buf->head];
        serial_getchar_buf->head = (serial_getchar_buf->head + 1) % sizeof(serial_getchar_buf->buf);
        switch (ch) {
        case 'a':
            if (!flag) {
                flag = 1;
                bench_from_emit();
            }
            break;
        case 'b':
            flag = 0;
            break;
        default:
            assert(!"What are you doing?");
            break;
        }
    }
}

uint64_t ccount = 0, tot = 0;
uint64_t prev, start, ts;

void count_idle(UNUSED void *arg)
{
    printf("Measurment starting...\n");
    sel4bench_init();
    start = (uint64_t)sel4bench_get_cycle_count();
    prev = start;

    while (flag) {
        ts = (uint64_t)sel4bench_get_cycle_count();
        uint64_t diff;

        /* handle overflow */
        if (ts < prev) {
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
    tot += ts - start;
    printf("idle/tot: %llu/%llu, idle proportion: %f%%\n", ccount, tot, ((double) ccount / (double) tot) * 100);
}

int run(void)
{
    bench_to_reg_callback(&count_idle, NULL);
    getchar_handler();

    return 0;
}
