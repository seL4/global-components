/*
 * Copyright 2022, DornerWorks
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

void getchar_emit(unsigned int id) WEAK;
seL4_Word getchar_enumerate_badge(unsigned int id) WEAK;
seL4_Word raw_putchar_enumerate_badge(unsigned int id) WEAK;
seL4_Word raw_batch_enumerate_badge(unsigned int id) WEAK;
seL4_Word processed_putchar_enumerate_badge(unsigned int id) WEAK;
seL4_Word processed_batch_enumerate_badge(unsigned int id) WEAK;
unsigned int getchar_num_badges() WEAK;
unsigned int raw_putchar_num_badges() WEAK;
unsigned int raw_batch_num_badges() WEAK;
unsigned int processed_putchar_num_badges() WEAK;
unsigned int processed_batch_num_badges() WEAK;
void *getchar_buf(seL4_Word client_id) WEAK;
void *processed_batch_buf(seL4_Word client_id) WEAK;
void *raw_batch_buf(seL4_Word client_id) WEAK;
int getchar_largest_badge(void) WEAK;
