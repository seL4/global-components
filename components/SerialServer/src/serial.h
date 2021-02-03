/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <platsupport/irq.h>

void serial_server_irq_handle(void *data, ps_irq_acknowledge_fn_t acknowledge_fn, void *ack_data);
