/*
 * Copyright 2018, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */
#include <stdio.h>
#include <stdint.h>

#include <camkes.h>
#include <sel4/sel4.h>
#include <utils/attribute.h>
#include <utils/ansi.h>

#include "../../plat.h"

/* configuration */
#define BAUD_RATE 115200

/* register layout. Done by offset from base port */
#define THR_ADDR (0)
#define RBR_ADDR (0)
#define LATCH_LOW_ADDR (0)
#define LATCH_HIGH_ADDR (1)
#define IER_ADDR (1)
#define FCR_ADDR (2)
#define IIR_ADDR (2)
#define LCR_ADDR (3)
#define MCR_ADDR (4)
#define LSR_ADDR (5)
#define MSR_ADDR (6)

#define IER_RESERVED_MASK (BIT(6) | BIT(7))

#define FCR_ENABLE BIT(0)
#define FCR_CLEAR_RECEIVE BIT(1)
#define FCR_CLEAR_TRANSMIT BIT(2)
#define FCR_TRIGGER_16_1 (0)

#define LCR_DLAB BIT(7)

#define MCR_DTR BIT(0)
#define MCR_RTS BIT(1)
#define MCR_AO1 BIT(2)
#define MCR_AO2 BIT(3)

#define LSR_EMPTY_DHR BIT(6)
#define LSR_EMPTY_THR BIT(5)
#define LSR_DATA_READY BIT(0)

#define IIR_FIFO_ENABLED (BIT(6) | BIT(7))
#define IIR_REASON (BIT(1) | BIT(2) | BIT(3))
#define IIR_MSR (0)
#define IIR_THR BIT(1)
#define IIR_RDA BIT(2)
#define IIR_TIME (BIT(3) | BIT(2))
#define IIR_LSR (BIT(2) | BIT(1))
#define IIR_PENDING BIT(0)

static int fifo_depth = 1;
static int fifo_used = 0;

static inline void write_ier(uint8_t val)
{
    serial_port_out8_offset(IER_ADDR, val);
}
static inline uint8_t read_ier()
{
    return serial_port_in8_offset(IER_ADDR);
}

static inline void write_lcr(uint8_t val)
{
    serial_port_out8_offset(LCR_ADDR, val);
}
static inline uint8_t read_lcr()
{
    return serial_port_in8_offset(LCR_ADDR);
}

static inline void write_fcr(uint8_t val)
{
    serial_port_out8_offset(FCR_ADDR, val);
}
/* you cannot read the fcr */

static inline void write_mcr(uint8_t val)
{
    serial_port_out8_offset(MCR_ADDR, val);
}

static inline uint8_t read_lsr()
{
    return serial_port_in8_offset(LSR_ADDR);
}

static inline uint8_t read_rbr()
{
    return serial_port_in8_offset(RBR_ADDR);
}

static inline void write_thr(uint8_t val)
{
    serial_port_out8_offset(THR_ADDR, val);
}

static inline uint8_t read_iir()
{
    return serial_port_in8_offset(IIR_ADDR);
}

static inline uint8_t read_msr()
{
    return serial_port_in8_offset(MSR_ADDR);
}

static void wait_for_fifo()
{
    while (! (read_lsr() & (LSR_EMPTY_DHR | LSR_EMPTY_THR)));
    fifo_used = 0;
}

/* assume DLAB == 1*/
static inline void write_latch_high(uint8_t val)
{
    serial_port_out8_offset(LATCH_HIGH_ADDR, val);
}
static inline void write_latch_low(uint8_t val)
{
    serial_port_out8_offset(LATCH_LOW_ADDR, val);
}

static void set_dlab(int v)
{
    if (v) {
        write_lcr(read_lcr() | LCR_DLAB);
    } else {
        write_lcr(read_lcr() & ~LCR_DLAB);
    }
}

static inline void write_latch(uint16_t val)
{
    set_dlab(1);
    write_latch_high(val >> 8);
    write_latch_low(val & 0xff);
    set_dlab(0);
}

static void disable_interrupt()
{
    write_ier(0);
}

static void disable_fifo()
{
    /* first attempt to use the clear fifo commands */
    write_fcr(FCR_CLEAR_TRANSMIT | FCR_CLEAR_RECEIVE);
    /* now disable with a 0 */
    write_fcr(0);
}

static void set_baud_rate(uint32_t baud)
{
    assert(baud != 0);
    assert(115200 % baud == 0);
    uint16_t divisor = 115200 / baud;
    write_latch(divisor);
}

static void reset_state()
{
    /* clear internal global state here */
    fifo_used = 0;
}

static void enable_fifo()
{
    /* check if there is a fifo and how deep it is */
    uint8_t info = read_iir();
    if ((info & IIR_FIFO_ENABLED) == IIR_FIFO_ENABLED) {
        fifo_depth = 16;
        write_fcr(FCR_TRIGGER_16_1 | FCR_ENABLE);
    } else {
        fifo_depth = 1;
    }
}

static void reset_lcr()
{
    /* set 8-n-1 */
    write_lcr(3);
}

static void reset_mcr()
{
    write_mcr(MCR_DTR | MCR_RTS | MCR_AO1 | MCR_AO2);
}

static void clear_iir(bool initialised, handle_char_fn handle_char)
{
    uint8_t iir;
    while (! ((iir = read_iir()) & IIR_PENDING)) {
        switch (iir & IIR_REASON) {
        case IIR_MSR:
            read_msr();
            break;
        case IIR_THR:
            break;
        case IIR_RDA:
        case IIR_TIME:
            while (initialised && handle_char && (read_lsr() & LSR_DATA_READY)) {
                handle_char(read_rbr());
            }
            break;
        case IIR_LSR:
            read_lsr();
            break;
        }
    }
}

static void enable_interrupt()
{
    write_ier(1);
}

ssize_t plat_serial_write(void *buf, size_t buf_size, chardev_callback_t cb, void *token)
{
    ZF_LOGF("%s is not implemented", __func__);
}

ssize_t plat_serial_read(void *buf, size_t buf_size, chardev_callback_t cb, void *token)
{
    ZF_LOGF("%s is not implemented", __func__);
}

void plat_serial_interrupt(handle_char_fn handle_char)
{
    clear_iir(true, handle_char);
}

void plat_serial_putchar(int c)
{
    /* check how much fifo we've used and if we need to drain it */
    if (fifo_used == fifo_depth) {
        wait_for_fifo();
    }
    write_thr((uint8_t)c);
    fifo_used++;
}

void plat_pre_init(void)
{
    set_dlab(0); // we always assume the dlab is 0 unless we explicitly change it
    disable_interrupt();
    disable_fifo();
    reset_lcr();
    reset_mcr();
    clear_iir(false, NULL);
    set_baud_rate(BAUD_RATE);
    reset_state();
    enable_fifo();
    enable_interrupt();
    clear_iir(false, NULL);
}
