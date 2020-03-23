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

#pragma once

#include <tx2bpmp/bpmp.h>

int bpmpserver_interface_init(void *bpmpserver_shared_mem, size_t shared_mem_size,
                              int (*bpmpserver_call)(int mrq, size_t tx_size, size_t *bytes_rxd),
                              struct tx2_bpmp *bpmp);
