#include <stddef.h>
#include "driver_usb_hs.h"
#include "custom_assert.h"

drv_usb_hs_status_t hw_usb_hs_init(const drv_usb_hs_t *usb_hs_handle)
{
    CUSTOM_ASSERT(usb_hs_handle != NULL);
    return usb_hs_handle->init(usb_hs_handle->hw_context);
}

drv_usb_hs_status_t hw_usb_hs_deinit(const drv_usb_hs_t *usb_hs_handle)
{
    CUSTOM_ASSERT(usb_hs_handle != NULL);
    return usb_hs_handle->deinit(usb_hs_handle->hw_context);
}
