#include <stddef.h>
#include "driver_rndis.h"
#include "custom_assert.h"

drv_rndis_status_t hw_rndis_init(const drv_rndis_t *rndis_handle)
{
    CUSTOM_ASSERT(rndis_handle != NULL);
    return rndis_handle->init(rndis_handle->hw_context);
}

drv_rndis_status_t hw_rndis_deinit(const drv_rndis_t *rndis_handle)
{
    CUSTOM_ASSERT(rndis_handle != NULL);
    return rndis_handle->deinit(rndis_handle->hw_context);
}