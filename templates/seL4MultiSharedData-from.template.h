/*#
 *#Copyright 2021, HENSOLDT Cyber GmbH
 *#
 *#SPDX-License-Identifier: BSD-2-Clause
  #*/


#include <sel4/sel4.h>

/*# Declare all the function prototypes for this connector. #*/
volatile void * /*? me.interface.name ?*/_get_buf(void);
size_t /*? me.interface.name ?*/_get_size(void);
