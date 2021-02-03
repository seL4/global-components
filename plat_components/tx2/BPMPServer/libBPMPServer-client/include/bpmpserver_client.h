/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <tx2bpmp/bpmp.h>

int bpmpserver_interface_init(void *bpmpserver_shared_mem, size_t shared_mem_size,
                              int (*bpmpserver_call)(int mrq, size_t tx_size, size_t *bytes_rxd),
                              struct tx2_bpmp *bpmp);
