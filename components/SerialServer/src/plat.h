/*
 * Copyright 2018, Data61
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

typedef void (*handle_char_fn)(uint8_t);

void plat_pre_init(void);
void plat_serial_interrupt(handle_char_fn handle_char);
void plat_serial_putchar(int c);
