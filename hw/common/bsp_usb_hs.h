#ifndef _BSP_USB_HS_H_
#define _BSP_USB_HS_H_

#include "driver_usb_hs.h"

extern const drv_usb_hs_t drv_usb_hs_0;

extern const drv_usb_hs_t *drv_usb_hs_array[];

#define DRV_USB_HS_COUNT 1
#define DRV_USB_HS_INSTANCE(n) (drv_usb_hs_array[n])

#endif // _BSP_USB_HS_H_
