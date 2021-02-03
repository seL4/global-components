/*#
 *#Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *#
 *#SPDX-License-Identifier: BSD-2-Clause
  #*/


#include <sel4/sel4.h>
#include <platsupport/gpio.h>
#include <platsupport/plat/gpio.h>

gpio_id_t */*? me.interface.name ?*/_get_allocated_pins(void);
unsigned int /*? me.interface.name ?*/_num_allocated_pins(void);
