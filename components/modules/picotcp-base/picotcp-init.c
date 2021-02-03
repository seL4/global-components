/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include <autoconf.h>

#include <string.h>
#include <sel4/sel4.h>
#include <platsupport/io.h>
#include <sel4utils/sel4_zf_logif.h>
#undef PACKED
#include <pico_stack.h>
#include <pico_socket.h>
#include <pico_addressing.h>
#include <pico_ipv4.h>
#include <pico_dhcp_client.h>
#include <pico_device.h>

#define PICO_TICK_MS 10


/*
 * Needed for the picotcp from pico_sel4.h
 * Although time is not needed for UDP, it is used in the arp update.
 */
volatile uint32_t pico_ms_tick = 0;

int clk_get_time(void)
{
    uint64_t time_in_ms = pico_ms_tick;
    return (time_in_ms & 0xFFFFFFFF);
}
uint64_t  pico_base_timer_time(void);
static void stack_tick_callback(UNUSED seL4_Word badge, void *cookie)
{
    pico_ms_tick = pico_base_timer_time() / NS_IN_MS;
    pico_stack_tick();
}



static uint32_t dhcp_client_xid;
static const char *multicast_addr;

static void dhcp_callback(void *cli, int code)
{
    struct pico_ip4 address, gateway, netmask, multicast, zero = {0};

    ZF_LOGF_IF(code == PICO_DHCP_ERROR, "Error occurred in pico DHCP negotiation: %s", strerror(pico_err))
    if (code == PICO_DHCP_SUCCESS) {
        address = pico_dhcp_get_address(cli);
        ZF_LOGD("ip addr = %x", address.addr);
        gateway = pico_dhcp_get_gateway(cli);
        netmask = pico_dhcp_get_netmask(cli);
        struct pico_device *dev = pico_get_device("eth0");
        if (dev == NULL) {
            ZF_LOGE("No device registered to call dhcp on");
        }

        pico_ipv4_route_add(zero, zero, gateway, 1, NULL);
        pico_ipv4_link_add(dev, address, netmask);

        pico_string_to_ipv4(multicast_addr, &multicast.addr);
        if (pico_ipv4_is_multicast(multicast.addr)) {
            ZF_LOGE("Multicast not yet implemented\n");
            // PicoTCP usually deals with multicast at the socket layer, using pico_socket_setoption.
            // It can be done at the igmp level too by using igmp_state_change. See the picoTCP documentation
            // Eg: pico_igmp_state_change(&ipaddr, &multicast, .... );
        }
    }
}

void eth_init_custom_ip(struct pico_device *dev, const char *ip_addr)
{
    struct pico_ip4 netmask, ipaddr, gw, multicast, zero = {0};

    pico_string_to_ipv4("0.0.0.0", &gw.addr);
    pico_string_to_ipv4(ip_addr, &ipaddr.addr);
    ZF_LOGD("ip addr = %x", ipaddr.addr);
    pico_string_to_ipv4(multicast_addr, &multicast.addr);
    pico_string_to_ipv4("255.255.255.0", &netmask.addr);
    pico_ipv4_route_add(zero, zero, gw, 1, NULL);

    pico_ipv4_link_add(dev, ipaddr, netmask);

    if (pico_ipv4_is_multicast(multicast.addr)) {
        ZF_LOGE("Multicast not yet implemented\n");
        // PicoTCP usually deals with multicast at the socket layer, using pico_socket_setoption.
        // It can be done at the igmp level too by using igmp_state_change. See the picoTCP documentation
        // Eg: pico_igmp_state_change(&ipaddr, &multicast, .... );
    }
}

typedef int (*register_callback_handler_fn_t)(seL4_Word badge, const char *,
                                              void (*callback_handler)(seL4_Word, void *), void *cookie);

int init_pico_post(ps_io_ops_t *io_ops, seL4_Word timer_badge, int (*timer_periodic)(int p_tid, uint64_t p_ns),
                   const char *ip_addr, const char *multicast_addr_, register_callback_handler_fn_t callback_handler)
{
    struct pico_device *dev = pico_get_device("eth0");
    if (dev == NULL) {
        ZF_LOGE("No device registered to call dhcp on");
    }

    multicast_addr = multicast_addr_;
    /* if ip_addr is configured, use it; otherwise get an IP address from DHCP */
    if (strlen(ip_addr)) {
        eth_init_custom_ip(dev, ip_addr);
    } else {
        ZF_LOGF_IF(pico_dhcp_initiate_negotiation(dev, &dhcp_callback, &dhcp_client_xid) != 0,
                   "Failed to initiate the DHCP negotiation");

    }

    callback_handler(timer_badge, "pico_server_tick", stack_tick_callback, NULL);
    /* Start the timer for the TCP stack */
    timer_periodic(0, NS_IN_MS * PICO_TICK_MS);
    return 0;

}
