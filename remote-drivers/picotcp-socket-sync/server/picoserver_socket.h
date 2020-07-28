/*
 * Copyright 2019, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(DATA61_GPL)
 */

#pragma once

#include <pico_socket.h>
#include <picoserver_event.h>

typedef struct picoserver_socket_async {
    /* Number of UDP packets queued, or 1 if any TCP data is pending */
    size_t rx_pending;
    /* Queued empty packets to receive data into */
    tx_msg_t *rx_pending_queue;
    tx_msg_t *rx_pending_queue_end;

    /* Pending packets to send */
    tx_msg_t *tx_pending_queue;
    tx_msg_t *tx_pending_queue_end;

} picoserver_socket_async_t;

typedef struct picoserver_socket {
    int protocol;
    int client_id;
    int socket_fd;
    struct pico_socket *socket;
    uint16_t events;

    /* This is set if the client sets the socket as async */
    picoserver_socket_async_t *async_transport;
} picoserver_socket_t;
