/*#
 *#Copyright 2020, Data61
 *#Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 *#ABN 41 687 119 230.
 *#
 *#This software may be distributed and modified according to the terms of
 *#the BSD 2-Clause license. Note that NO WARRANTY is provided.
 *#See "LICENSE_BSD2.txt" for details.
 *#
 *#@TAG(DATA61_BSD)
  #*/

#include <sel4/sel4.h>
#include <platsupport/gpio.h>
#include <platsupport/plat/gpio.h>

gpio_id_t */*? me.interface.name ?*/_get_allocated_pins(void);
unsigned int /*? me.interface.name ?*/_num_allocated_pins(void);
