/*
 * Copyright 2019, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(DATA61_GPL)
 */

#include <autoconf.h>

#include <string.h>
#include <sel4/sel4.h>
#include <platsupport/io.h>
#include <sel4utils/sel4_zf_logif.h>
#include <lwip/init.h>
#include <lwip/dhcp.h>
#include <lwip/ip_addr.h>
#include <lwip/netif.h>
#include <lwip/timeouts.h>
#include <sel4bench/sel4bench.h>

#define LWIP_TICK_MS 10

bool timers_initialised;

uint64_t (*timer_get_time_fn)(void);

u32_t sys_now(void)
{
    if (!timers_initialised) {
        /* lwip_init() will call this when initialising its own timers,
         * but the timer RPC is not set up at this point so just return 0 */
        return 0;
    } else {
        uint64_t time_now = timer_get_time_fn();
        return time_now / NS_IN_MS;
    }
}

static void stack_tick_callback(UNUSED seL4_Word badge, void *cookie)
{
    sys_check_timeouts();
}

static void eth_init_custom_ip(struct netif *netif, const char *ip_addr, const char *multicast)
{
    struct ip4_addr netmask, ipaddr;

    ipaddr_aton(ip_addr, &ipaddr);

    netif_set_ipaddr(netif, &ipaddr);

    if (multicast) {
        ZF_LOGE("Multicast is currently not supported");
    }

    printf("%s lwIP netif is set up on %s\n", netif->name, ip_addr);
}

static void netif_status_callback(struct netif *netif)
{
    ZF_LOGW("Netif status callback");
    if (dhcp_supplied_address(netif)) {
        printf("DHCP request finished, IP address for netif %s is: %s\n",
               netif->name, ip4addr_ntoa(netif_ip4_addr(netif)));
    }
}

int init_lwip_pre(void)
{
    lwip_init();

    return 0;
}

typedef int (*register_callback_handler_fn_t)(seL4_Word badge, const char *,
                                              void (*callback_handler)(seL4_Word, void *),
                                              void *cookie);

int init_lwip_post(ps_io_ops_t *io_ops, seL4_Word timer_badge, int (*timer_periodic)(int p_tid, uint64_t p_ns), uint64_t (*timer_get_time)(void),
                   const char *ip_addr, const char *multicast_addr_, register_callback_handler_fn_t callback_handler)
{
    timers_initialised = true;

    ZF_LOGW("Init lwip post");

    struct netif *netif = netif_find("e0");
    if (netif == NULL) {
        ZF_LOGE("No device registered to call dhcp on or start");
    }

    timer_get_time_fn = timer_get_time;

    /* if ip_addr is configured, use it; otherwise get an IP address from DHCP */
    if (strlen(ip_addr)) {
        eth_init_custom_ip(netif, ip_addr, multicast_addr_);
        netif_set_up(netif);
    } else {
        ZF_LOGW("No ip addr configured. Getting an IP address from DHCP");
        netif_set_status_callback(netif, netif_status_callback);
        netif_set_up(netif);
        ZF_LOGF_IF(dhcp_start(netif), "Failed to initiate the DHCP negotiation");
    }

    //callback_handler(timer_badge, "lwip_stack_tick_callback", stack_tick_callback, NULL);
    callback_handler(0, "lwip_stack_tick_callback", stack_tick_callback, NULL);
    /* Start the timer for the TCP stack */
    timer_periodic(0, NS_IN_MS * LWIP_TICK_MS);

    return 0;
}
