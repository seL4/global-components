/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#pragma once

typedef struct picoserver_peer {
    int result;
    int socket;
    uint32_t peer_addr;
    uint16_t peer_port;
} picoserver_peer_t;
