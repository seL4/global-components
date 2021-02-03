/*
 * Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include <autoconf.h>

#include <pico_device.h>
#include <pico_stack.h>
#undef PACKED
#include <string.h>
#include <camkes.h>
#include <pico_ipv4.h>
#include <sel4/sel4.h>
#include <sel4utils/sel4_zf_logif.h>
#include <ethdrivers/raw.h>

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
        if (status == 0) {
            break;
        }
    }
    return loop_score;
}

struct pico_device _pico_driver;
struct pico_device *pico_driver = &_pico_driver;

int eth_init(ps_io_ops_t *io_ops)
{

    /* Create a driver. This utilises preallocated buffers, backed up by malloc above */
    /* Attach funciton pointers */
    pico_driver->send = pico_eth_send;
    pico_driver->poll = pico_eth_poll;

    /* Configure the mtu in picotcp */
    uint8_t mac[6] = {0};
    low_level_init(mac, &pico_driver->mtu);
    if (pico_device_init(pico_driver, "eth0", mac) != 0) {
        ZF_LOGF("Failed to initialize pico device");
        return -1;
    }

    return 0;
}

CAMKES_PRE_INIT_MODULE_DEFINE(eth_init_reg, eth_init);
