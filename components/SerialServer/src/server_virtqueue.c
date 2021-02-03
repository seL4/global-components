/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <autoconf.h>
#include <camkes.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <sel4/sel4.h>
#include <utils/attribute.h>
#include <utils/ansi.h>
#include <camkes.h>
#include <virtqueue.h>
#include <camkes/virtqueue.h>
#include <platsupport/chardev.h>
#include "plat.h"
#include "server_virtqueue.h"

virtqueue_device_t read_virtqueue;
virtqueue_device_t write_virtqueue;

int write_in_progress;
int read_in_progress;

enum virtqueue_op {
    VQ_SERIAL_READ,
    VQ_SERIAL_WRITE
};

typedef struct {
    virtqueue_device_t *vq;
    virtqueue_ring_object_t virtqueue_handle;
    void *serial_buf;
    size_t buf_size;
} virtqueue_token_t;

virtqueue_token_t current_read_vq_token;
virtqueue_token_t current_write_vq_token;

static void write_callback(ps_chardevice_t *device, enum chardev_status stat,
                           size_t bytes_transfered, void *token)
{
    virtqueue_token_t *vq_token = token;
    void *serial_buf = vq_token->serial_buf;
    if (!serial_buf) {
        return;
    }
    virtqueue_device_t *vq = vq_token->vq;
    vq->notify();
    free(serial_buf);
    vq_token->serial_buf = NULL;
    write_in_progress = 0;
}

static void read_callback(ps_chardevice_t *device, enum chardev_status stat,
                          size_t bytes_transfered, void *token)
{
    virtqueue_token_t *vq_token = token;
    void *serial_buf = vq_token->serial_buf;
    size_t buf_size = vq_token->buf_size;

    if (!serial_buf) {
        return;
    }

    virtqueue_device_t *vq = vq_token->vq;
    virtqueue_ring_object_t handle = vq_token->virtqueue_handle;

    if (camkes_virtqueue_device_scatter_copy_buffer(vq, &handle, serial_buf, buf_size) < 0) {
        ZF_LOGE("Unable to copy read data to virtqueue");
    }

    vq->notify();
    free(serial_buf);
    vq_token->serial_buf = NULL;

    read_in_progress = 0;
}

static void handle_virtqueue_message(virtqueue_device_t *vq, virtqueue_ring_object_t *handle, enum virtqueue_op op)
{
    void *serial_buffer;
    size_t buf_size;

    buf_size = virtqueue_scattered_available_size(vq, handle);
    if (!(serial_buffer = calloc(buf_size, sizeof(char)))) {
        ZF_LOGE("Unable to alloc serial buffer of size: %zu", buf_size);
        virtqueue_add_used_buf(vq, handle, 0);
        return;
    }

    if (op == VQ_SERIAL_READ) {
        current_read_vq_token.vq = vq;
        current_read_vq_token.virtqueue_handle = *handle;
        current_read_vq_token.serial_buf = serial_buffer;
        current_read_vq_token.buf_size = buf_size;
        if (plat_serial_read(serial_buffer, buf_size, read_callback, &current_read_vq_token) < 0) {
            ZF_LOGE("Unable to serial read buffer");
            free(serial_buffer);
            virtqueue_add_used_buf(vq, handle, 0);
        }
    } else {
        int err;
        current_write_vq_token.vq = vq;
        current_write_vq_token.virtqueue_handle = *handle;
        current_write_vq_token.serial_buf = serial_buffer;
        current_write_vq_token.buf_size = buf_size;
        camkes_virtqueue_device_gather_copy_buffer(vq, handle, serial_buffer, buf_size);
        if ((err = plat_serial_write(serial_buffer, buf_size, write_callback, &current_write_vq_token)) < 0) {
            ZF_LOGE("Unable to serial write buffer: %d", err);
            free(serial_buffer);
            virtqueue_add_used_buf(vq, handle, 0);
        }
    }
    return;
}

static void handle_virtqueue_callback(virtqueue_device_t *vq, enum virtqueue_op op)
{
    virtqueue_ring_object_t handle;

    if (!virtqueue_get_available_buf(vq, &handle)) {
        ZF_LOGE("Serial server virtqueue dequeue failed");
        return;
    }

    /* Process the incoming virtqueue message */
    handle_virtqueue_message(vq, &handle, op);
}

void serial_read_wait_callback(void)
{
    int UNUSED error;
    error = serial_lock();
    if (VQ_DEV_POLL(&read_virtqueue)) {
        read_in_progress = 1;
        handle_virtqueue_callback(&read_virtqueue, VQ_SERIAL_READ);
    }
    error = serial_unlock();
}

void serial_write_wait_callback(void)
{
    int UNUSED error;
    error = serial_lock();
    if (VQ_DEV_POLL(&write_virtqueue)) {
        write_in_progress = 1;
        handle_virtqueue_callback(&write_virtqueue, VQ_SERIAL_WRITE);
    }
    error = serial_unlock();
}

int virtqueue_init(void)
{
    /* Initialise read virtqueue */
    int error = camkes_virtqueue_device_init(&read_virtqueue, 0);
    if (error) {
        ZF_LOGE("Unable to initialise serial server read virtqueue");
    }
    /* Initialise write virtqueue */
    error = camkes_virtqueue_device_init(&write_virtqueue, 1);
    if (error) {
        ZF_LOGE("Unable to initialise serial server write virtqueue");
    }

    return error;
}
