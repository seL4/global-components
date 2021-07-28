/*#
 *#Copyright 2021, HENSOLDT Cyber GmbH
 *#
 *#SPDX-License-Identifier: BSD-2-Clause
  #*/

#include <sel4/sel4.h>

void * /*? me.interface.name ?*/_buf(seL4_Word client_id);
size_t /*? me.interface.name ?*/_buf_size(seL4_Word client_id);
unsigned int /*? me.interface.name ?*/_num_badges(void);
seL4_Word /*? me.interface.name ?*/_enumerate_badge(unsigned int i);
