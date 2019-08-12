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

#include <camkes.h>
#include <camkes/io.h>
#include <camkes/dma.h>
#include <ethdrivers/raw.h>
#include <ethdrivers/tx2.h>
#include <platsupport/io.h>
#include <platsupport/clock.h>
#include <platsupport/irq.h>
#include <vka/vka.h>
#include <simple/simple.h>
#include <allocman/vka.h>
#include <sel4utils/vspace.h>

#include "../../ethdriver.h"

static ps_irq_t irq_info;

int ethif_preinit(vka_t *vka, simple_t *camkes_simple, vspace_t *vspace,
                  ps_io_ops_t *io_ops)
{
    return camkes_io_ops(io_ops);
}

int ethif_init(struct eth_driver *eth_driver, ps_io_ops_t *io_ops, ps_irq_ops_t *irq_ops)
{
    struct arm_eth_plat_config eth_config = (struct arm_eth_plat_config) {
        .buffer_addr = (void *) EthDriver_0,
        .prom_mode = (uint8_t) promiscuous_mode
    };

    int error = ethif_tx2_init(eth_driver, *io_ops, (void *) &eth_config);
    if (error) {
        return error;
    }

    irq_info = (ps_irq_t) {
        .type = PS_INTERRUPT, .irq = { .number = TX2_INT_COMMON_ETHER_QOS }
    };
    irq_id_t irq_id = ps_irq_register(irq_ops, irq_info, eth_irq_handle, &irq_info);
    if (irq_id < 0) {
        return -1;
    }
    ps_irq_t irq_info1 = { .type = PS_INTERRUPT, .irq = { .number = TX2_INT_RX0_ETHER_QOS }};
    irq_id_t irq_id1 = ps_irq_register(irq_ops, irq_info1, eth_irq_handle, &irq_info1);
    if (irq_id1 < 0) {
        return -1;
    }

    return 0;
}
