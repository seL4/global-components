/*#
 *#Copyright 2021, HENSOLDT Cyber GmbH
 *#
 *#SPDX-License-Identifier: BSD-2-Clause
  #*/


#include <sel4/sel4.h>

/*- include 'seL4RPCDataport-to.template.h' -*/

/*# Declare all the function prototypes for this connector. #*/
void /*? me.interface.name ?*/_emit(unsigned int badge);
bool /*? me.interface.name ?*/_has_mac(unsigned int badge);
void /*? me.interface.name ?*/_get_mac(unsigned int badge, uint8_t *mac);
