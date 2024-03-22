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

drv_rndis_status_t hw_rndis_tx(const drv_rndis_t *rndis_handle, const void *buf, size_t len, size_t *written)
{
    CUSTOM_ASSERT(rndis_handle != NULL);
    if (written == NULL || buf == NULL)
    {
        return DRV_RNDIS_STATUS_ERROR;
    }
    return rndis_handle->tx(rndis_handle->hw_context, buf, len, written);
}

drv_rndis_status_t hw_rndis_up(const drv_rndis_t *rndis_handle, bool *is_up)
{
    CUSTOM_ASSERT(rndis_handle != NULL);
    if (is_up == NULL)
    {
        return DRV_RNDIS_STATUS_ERROR;
    }
    return rndis_handle->up(rndis_handle->hw_context, is_up);
}
