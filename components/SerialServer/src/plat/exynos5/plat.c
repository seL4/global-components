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

#include <sel4platsupport/serial.h>
#include <platsupport/chardev.h>

#define UART2_ADDR (unsigned int)serial_mem

struct ps_chardevice serial_device;
struct ps_chardevice* serial = NULL;

ssize_t plat_serial_write(void *buf, size_t buf_size, chardev_callback_t cb, void *token)
{
    ssize_t res = ps_cdev_write(serial, buf, buf_size, cb, token);
    return res;
}

ssize_t plat_serial_read(void *buf, size_t buf_size, chardev_callback_t cb, void *token)
{
    ssize_t res = ps_cdev_read(serial, buf, buf_size, cb, token);
    return res;
}

void plat_serial_interrupt(handle_char_fn handle_char)
{
    if (serial) {
        ps_cdev_handle_irq(serial, 0);
        char data = ps_cdev_getchar(serial);
        /*
         * Filtering out spurious 255 (nbsp) characters as this
         * impacts the serial server state machine
         */
        if (data != EOF && data != 255) {
            handle_char(data);
        }
    }
}

void plat_serial_putchar(int c)
{
    if (serial) {
        ps_cdev_putchar(serial, c);
    }
}

void plat_pre_init(void)
{
    struct ps_chardevice temp_device;

    static_serial_params_t serial_params;
    serial_params.vaddr = (void *)UART2_ADDR;
    serial_params.clock_id = CLK_UART2;
    serial_params.uart_mux_feature = MUX_UART2;

    ps_io_ops_t ops;
    int error = ps_new_stdlib_malloc_ops(&ops.malloc_ops);
    ZF_LOGF_IF(error, "Failed to get malloc ops");

    if (ps_cdev_static_init(&ops, &temp_device, (void *)&serial_params)) {
        serial_device = temp_device;
        serial = &serial_device;
    } else {
        ZF_LOGE("Failed to initialise character device");
    }
}
