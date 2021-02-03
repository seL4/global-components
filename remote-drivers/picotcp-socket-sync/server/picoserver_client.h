/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#pragma once

#include "khash.h"
#include <sel4/sel4.h>
#undef PACKED
#include "picoserver_socket.h"
#include <picoserver_event.h>

/* Declare a hash map to keep track of sockets by ID*/
KHASH_MAP_INIT_INT(socket, picoserver_socket_t *)

/* Declare a hash map to keep track of sockets by their address,
 * used to find the correct socket structure in the PicoTCP socket callbacks
 * (only the address of the socket is given, and we can't pass cookies) */
KHASH_MAP_INIT_INT64(socket_addr, picoserver_socket_t *);

/* Declare a hash set to keep track of which sockets have events */
KHASH_SET_INIT_INT(socket_event);

typedef struct picoserver_client {
    khint32_t bump_index;                    // Used to help keep track of the next free socket FD
    khash_t(socket) *socket_table;
    khash_t(socket_event) *socket_event_set;
} picoserver_client_t;

/*
 * Initialises each of the client's bookkeeping structures, i.e. their
 * picoserver_client_t struct.
 */
void picoserver_clients_init(int num_clients);

/*
 * Gets the number of sockets currently assigned to a particular client.
 */
uint32_t client_get_num_sockets(seL4_Word client_id);

/*
 * Gets the picoserver_socket_t struct that belongs to a particular socket ID
 * for a client.
 */
picoserver_socket_t *client_get_socket(seL4_Word client_id, int socket_id);

/*
 * Gets the picoserver_socket_t struct that houses a particular pico_socket
 * struct.
 *
 * Used in socket_cb() in picoserver.c.
 */
picoserver_socket_t *client_get_socket_by_addr(struct pico_socket *socket_addr);

/*
 * Finds an empty socket ID and adds a new entry to a client's socket table.
 */
int client_put_socket(seL4_Word client_id, picoserver_socket_t *new_socket);

/*
 * Deletes all bookkeeping structures related to a client's assigned socket.
 */
int client_delete_socket(seL4_Word client_id, int socket_id);

/*
 * Populates the given picoserver_event_t struct with an outstanding socket
 * event for a client.
 */
void client_get_event(seL4_Word client_id, picoserver_event_t *ret_event);

/*
 * Adds an outstanding socket event to a client's event set.
 */
int client_put_event(seL4_Word client_id, int socket_id, uint16_t event);
