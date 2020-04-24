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
#include <camkes.h>
#include <ethdrivers/pico_dev_eth.h>
#include <pico_dhcp_client.h>
#include <pico_ipv4.h>
#include <sel4/sel4.h>
#include <sel4utils/sel4_zf_logif.h>

#include "pico_common.h"

/* 1500 is the standard ethernet MTU at the network layer. */
#define ETHER_MTU 1500
static void low_level_init(uint8_t *mac, int *mtu)
{
    *mtu = ETHER_MTU;
    ethdriver_mac(&mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
}

extern void *ethdriver_buf;


static int pico_eth_send(struct pico_device *dev, void *input_buf, int len)
{

    assert(len <= ETHER_MTU);

    memcpy((void *)ethdriver_buf, input_buf, len);

    int status = ethdriver_tx(len);

    switch (status) {
    case ETHIF_TX_FAILED:
        return 0; // Error for PICO
    case ETHIF_TX_COMPLETE:
    case ETHIF_TX_ENQUEUED:
        break;
    }

    return len;

}

/* Async driver will set a flag to signal that there is work to be done  */
static int pico_eth_poll(struct pico_device *dev, int loop_score)
{
    int len;
    while (loop_score > 0) {
        int status = ethdriver_rx(&len);
        if (status == -1) {
            break;
        }

        pico_stack_recv(dev, (void *)ethdriver_buf, len);
        loop_score--;
        if (status== 0) {
            break;
        }
    }
    return loop_score;
}

struct pico_device _pico_driver;
struct pico_device *pico_driver = &_pico_driver;
uint32_t dhcp_client_xid;
bool dhcp_negotiating = false;

uint32_t get_ipv4(void)
{
    void *cli = pico_dhcp_get_identifier(dhcp_client_xid);
    struct pico_ip4 address;

    if (cli) {
        address = pico_dhcp_get_address(cli);
    } else {
        pico_string_to_ipv4(ip_addr, &address.addr);
    }
    return address.addr;
}

void eth_init_continue(void *cli, int code)
{
    struct pico_ip4 address, gateway, netmask, multicast, zero = {0};
    char s_address[16] = { }, s_gateway[16] = { }, s_netmask[16] = { };

    ZF_LOGF_IF(code == PICO_DHCP_ERROR, "Error occurred in pico DHCP negotiation: %s", strerror(pico_err))

    if (code == PICO_DHCP_SUCCESS) {
        address = pico_dhcp_get_address(cli);
        ZF_LOGD("ip addr = %x", address.addr);
        gateway = pico_dhcp_get_gateway(cli);
        netmask = pico_dhcp_get_netmask(cli);

        pico_ipv4_route_add(zero, zero, gateway, 1, NULL);
        pico_ipv4_link_add(pico_driver, address, netmask);

        pico_string_to_ipv4(multicast_addr, &multicast.addr);
        if (pico_ipv4_is_multicast(multicast.addr)) {
            ZF_LOGE("Multicast not yet implemented\n");
            // PicoTCP usually deals with multicast at the socket layer, using pico_socket_setoption.
            // It can be done at the igmp level too by using igmp_state_change. See the picoTCP documentation
            // Eg: pico_igmp_state_change(&ipaddr, &multicast, .... );
        }

        /* DHCP negotiation done, the picotcp lock is going to be released after this function return */
        dhcp_negotiating = false;
    }
}

void eth_init_custom_ip(void)
{
    struct pico_ip4 netmask, ipaddr, gw, multicast, zero = {0};

    pico_string_to_ipv4("0.0.0.0", &gw.addr);
    pico_string_to_ipv4(ip_addr, &ipaddr.addr);
    ZF_LOGD("ip addr = %x", ipaddr.addr);
    pico_string_to_ipv4(multicast_addr, &multicast.addr);
    pico_string_to_ipv4("255.255.255.0", &netmask.addr);
    pico_ipv4_route_add(zero, zero, gw, 1, NULL);
    pico_ipv4_link_add(pico_driver, ipaddr, netmask);

    if (pico_ipv4_is_multicast(multicast.addr)) {
        ZF_LOGE("Multicast not yet implemented\n");
        // PicoTCP usually deals with multicast at the socket layer, using pico_socket_setoption.
        // It can be done at the igmp level too by using igmp_state_change. See the picoTCP documentation
        // Eg: pico_igmp_state_change(&ipaddr, &multicast, .... );
    }
}


void eth_init(void)
{


    /* Initialise the PicoTCP stack */
    pico_stack_init();

    /* Create a driver. This utilises preallocated buffers, backed up by malloc above */
    /* Attach funciton pointers */
    pico_driver->send = pico_eth_send;
    pico_driver->poll = pico_eth_poll;

    /* Configure the mtu in picotcp */
    uint8_t mac[6] = {0};
    low_level_init(mac, &pico_driver->mtu);
    if (pico_device_init(pico_driver, "eth0", mac) != 0) {
        ZF_LOGF("Failed to initialize pico device");
        return;
    }

    /* if ip_addr is configured, use it; otherwise get an IP address from DHCP */
    if (strlen(ip_addr)) {
        eth_init_custom_ip();
    } else {
        ZF_LOGF_IF(pico_dhcp_initiate_negotiation(pico_driver, &eth_init_continue, &dhcp_client_xid) != 0,
                   "Failed to initiate the DHCP negotiation");

        /* DHCP negotiation in progress */
        dhcp_negotiating = true;
        picotcp_lock();
    }
}
