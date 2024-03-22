#ifndef _DRIVER_RNDIS_H_
#define _DRIVER_RNDIS_H_

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
    } drv_rndis_t;

    drv_rndis_status_t hw_rndis_init(const drv_rndis_t *rndis_handle);
    drv_rndis_status_t hw_rndis_deinit(const drv_rndis_t *rndis_handle);

#ifdef __cplusplus
}
#endif

#endif // _DRIVER_RNDIS_H_