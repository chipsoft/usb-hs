#ifndef _BSP_RNDIS_MONGOOSE_H_
#define _BSP_RNDIS_MONGOOSE_H_

#include "driver_rndis.h"
#include "mongoose.h"

extern const drv_rndis_t drv_rndis_0;

extern const drv_rndis_t *drv_rndis_array[];

#define DRV_RNDIS_COUNT 1
#define DRV_RNDIS_INSTANCE(n) (drv_rndis_array[n])

#ifdef __cplusplus
extern "C"
{
#endif

// Driver function for Mongoose & RNDIS
size_t usb_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp);
bool usb_up(struct mg_tcpip_if *ifp);

#ifdef __cplusplus
}
#endif

#endif // _BSP_RNDIS_MONGOOSE_H_
