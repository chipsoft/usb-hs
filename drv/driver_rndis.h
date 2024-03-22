#ifndef _DRIVER_RNDIS_H_
#define _DRIVER_RNDIS_H_
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        DRV_RNDIS_STATUS_OK = 0,    ///< Success
        DRV_RNDIS_STATUS_ERROR = 1, ///< Generic error
    } drv_rndis_status_t;

    typedef struct
    {
        const void *hw_context;
        drv_rndis_status_t (*init)(const void *hw_context);
        drv_rndis_status_t (*deinit)(const void *hw_context);
        drv_rndis_status_t (*tx)(const void *hw_context, const void *buf, size_t len, size_t *written);
        drv_rndis_status_t (*up)(const void *hw_context, bool *is_up);
    } drv_rndis_t;

    drv_rndis_status_t hw_rndis_init(const drv_rndis_t *rndis_handle);
    drv_rndis_status_t hw_rndis_deinit(const drv_rndis_t *rndis_handle);
    drv_rndis_status_t hw_rndis_tx(const drv_rndis_t *rndis_handle, const void *buf, size_t len, size_t *written);
    drv_rndis_status_t hw_rndis_up(const drv_rndis_t *rndis_handle, bool *is_up);

#ifdef __cplusplus
}
#endif

#endif // _DRIVER_RNDIS_H_