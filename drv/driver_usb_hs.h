#ifndef _DRIVER_USB_HS_H_
#define _DRIVER_USB_HS_H_

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        DRV_USB_HS_STATUS_OK = 0,    ///< Success
        DRV_USB_HS_STATUS_ERROR = 1, ///< Generic error
    } drv_usb_hs_status_t;

    typedef struct
    {
        const void *hw_context;
        drv_usb_hs_status_t (*init)(const void *hw_context);
        drv_usb_hs_status_t (*deinit)(const void *hw_context);
    } drv_usb_hs_t;

    drv_usb_hs_status_t hw_usb_hs_init(const drv_usb_hs_t *usb_hs_handle);
    drv_usb_hs_status_t hw_usb_hs_deinit(const drv_usb_hs_t *usb_hs_handle);

#ifdef __cplusplus
}
#endif

#endif // _DRIVER_USB_HS_H_
