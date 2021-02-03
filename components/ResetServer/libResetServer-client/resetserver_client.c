/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <errno.h>

#include <utils/util.h>
#include <resetserver_client.h>

typedef struct resetserver_interface_data {
    int (*resetserver_assert)(reset_id_t id);
    int (*resetserver_deassert)(reset_id_t id);
} resetserver_interface_data_t;

static resetserver_interface_data_t resetserver_private_data;

static int resetserver_assert_wrapper(void *data, reset_id_t id)
{
    resetserver_interface_data_t *resetserver_data = data;

    return resetserver_data->resetserver_assert(id);
}

static int resetserver_deassert_wrapper(void *data, reset_id_t id)
{
    resetserver_interface_data_t *resetserver_data = data;

    return resetserver_data->resetserver_deassert(id);
}

int resetserver_interface_init(int (*resetserver_assert)(reset_id_t id),
                               int (*resetserver_deassert)(reset_id_t id),
                               reset_sys_t *reset_sys)
{
    if (!resetserver_assert) {
        ZF_LOGE("reset_server_assert is NULL");
        return -EINVAL;
    }

    if (!resetserver_deassert) {
        ZF_LOGE("reset_server_deassert is NULL");
        return -EINVAL;
    }

    if (!reset_sys) {
        ZF_LOGE("reset is NULL");
        return -EINVAL;
    }

    resetserver_private_data.resetserver_assert = resetserver_assert;
    resetserver_private_data.resetserver_deassert = resetserver_deassert;

    reset_sys->data = &resetserver_private_data;
    reset_sys->reset_assert = resetserver_assert_wrapper;
    reset_sys->reset_deassert = resetserver_deassert_wrapper;

    return 0;
}
