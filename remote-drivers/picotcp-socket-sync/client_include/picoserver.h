/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#pragma once


/*
 * Reference only the necessary constants and functions to clients that are
 * connected the PicoServer component.
 *
 * These constants and functions cover all the functionality that the
 * PicoServer currently provides.
 */
#ifndef PICO_SOCK_EV_RD
#define PICO_SOCK_EV_RD 1u
#define PICO_SOCK_EV_WR 2u
#define PICO_SOCK_EV_CONN 4u
#define PICO_SOCK_EV_CLOSE 8u
#define PICO_SOCK_EV_FIN 0x10u
#define PICO_SOCK_EV_ERR 0x80u
#define PICO_SHUT_RD   1
#define PICO_SHUT_WR   2
#define PICO_SHUT_RDWR 3
#define PICO_IPV4_INADDR_ANY 0x00000000U
#endif

#define PICOSERVER_READ PICO_SOCK_EV_RD
#define PICOSERVER_WRITE PICO_SOCK_EV_WR
#define PICOSERVER_CONN PICO_SOCK_EV_CONN
#define PICOSERVER_CLOSE PICO_SOCK_EV_CLOSE
#define PICOSERVER_FIN PICO_SOCK_EV_FIN
#define PICOSERVER_ERR PICO_SOCK_EV_ERR
#define PICOSERVER_IP_AVAIL (PICO_SOCK_EV_ERR << 1)

#define PICOSERVER_SHUT_RD PICO_SHUT_RD
#define PICOSERVER_SHUT_WR PICO_SHUT_WR
#define PICOSERVER_SHUT_RDWR PICO_SHUT_RDWR

#define PICOSERVER_ANY_ADDR_IPV4 PICO_IPV4_INADDR_ANY

/*
 * These functions will convert an IP address in string form to a integer
 * bitfield in network order format (1.2.3.4 -> 0x04030201), and vice versa.
 *
 * When converting a IPv4 address to a string, depending on the IP address, the
 * length of the buffer will have to be at most 16 bytes long which includes
 * the NULL byte at the end.
 */
int pico_ipv4_to_string(char *ipbuf, const uint32_t ip);
int
pico_string_to_ipv4(const char *ipstr, uint32_t *ip);
