/*
 * Copyright 2020, Data61, ABN 41 687 119 230.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <autoconf.h>
#include <camkes.h>
#include <camkes/dma.h>
#include <platsupport/io.h>
#include <platsupport/irq.h>
#include <vka/vka.h>
#include <simple/simple.h>
#include <simple/simple_helpers.h>
#include <allocman/vka.h>
#include <sel4utils/vspace.h>
#include <sel4utils/iommu_dma.h>
#include <sel4platsupport/arch/io.h>

seL4_CPtr(*original_vspace_get_cap)(vspace_t *, void *);

/* Returns the cap to the frame mapped to vaddr, assuming
 * vaddr points inside our dma pool. */
seL4_CPtr get_dma_frame_cap(vspace_t *vspace, void *vaddr)
{
    seL4_CPtr cap = camkes_dma_get_cptr(vaddr);
    if (cap == seL4_CapNull) {
        return original_vspace_get_cap(vspace, vaddr);
    }
    return cap;
}

/* Force the _dma_pools section to be created even if no modules are defined. */
static USED SECTION("_dma_pools") struct {} dummy_dma_pool;
/* Definitions so that we can find the exposed DMA frames */
extern dma_pool_t *__start__dma_pools[];
extern dma_pool_t *__stop__dma_pools[];


static uintptr_t dma_pin(void *cookie, void *addr, size_t size)
{
    return (uintptr_t)addr;
}


int pc99_iospace_setup(ps_io_ops_t *io_ops, vka_t *vka, simple_t *camkes_simple, vspace_t *vspace)
{
    int error = 0;
    int pci_bdf_int = 0;
    int bus, dev, fun = 0;
    uint16_t iospace_id = 0;
    /* component attribute describing iospace */
    const char *iospace_config = "/*? configuration[me.instance.name].get('iospaces', '')?*/";
    if (strlen(iospace_config) == 0) {
    	return 0;
    }
    ps_dma_man_t iospace_dma = {0};
    cspacepath_t iospace = {0};
    error = vka_cspace_alloc_path(vka, &iospace);
    if (error) {
        return error;
    }
    sscanf(iospace_config, "%hx:%x:%x.%d", &iospace_id, &bus, &dev, &fun);
    pci_bdf_int = bus * 256 + dev * 8 + fun;
    /* get this from the configuration */
    error = simple_get_iospace(camkes_simple, iospace_id, pci_bdf_int, &iospace);
    if (error) {
    	ZF_LOGE("Could not locate IOSpace: %s\n", iospace_config);
        return error;
    }

    /* Save a pointer to the original get_cap function for our vspace */
    original_vspace_get_cap = vspace->get_cap;

    /* The iommu driver needs the caps to frames backing the dma buffer.
     * It will invoke the get_cap method of its vspace to get these caps.
     * Since the vspace we give to the iommu driver wasn't used to allocate
     * the dma buffer, it doesn't know the caps to the frames backing the
     * buffer. CAmkES allocated the buffer statically, and so the caps are
     * known to it. Here, we override the get_cap method of our vspace to
     * return dma buffer frame caps provided by CAmkES. */
    vspace->get_cap = get_dma_frame_cap;

    error = sel4utils_make_iommu_dma_alloc(vka, vspace, &iospace_dma, 1, &iospace.capPtr);
    if (error) {
        return error;
    }

    for (dma_pool_t **pool = __start__dma_pools; pool < __stop__dma_pools; pool++) {
        for (size_t frame_id = 0; frame_id < (*pool)->num_frames; frame_id++) {
            dma_frame_t *frame = (*pool)->dma_frames[frame_id];
            int error = sel4utils_iommu_dma_alloc_iospace(iospace_dma.cookie, (void *)frame->vaddr, frame->size);
            if (error) {
                ZF_LOGE("Failed to setup IOMMU for DMA region: (%p, 0x%lx bytes)", (void *)frame->vaddr, frame->size);
                return error;
            }
        }
    }

    io_ops->dma_manager.dma_pin_fn = dma_pin;


    return 0;
}

CAMKES_DYNAMIC_INIT_MODULE_DEFINE(pc99_iospace_setup)
