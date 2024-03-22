#ifndef _BSP_RNDIS_H_
#define _BSP_RNDIS_H_

#include "driver_rndis.h"

extern const drv_rndis_t drv_rndis_0;

extern const drv_rndis_t *drv_rndis_array[];

#define DRV_RNDIS_COUNT 1
#define DRV_RNDIS_INSTANCE(n) (drv_rndis_array[n])

#endif // _BSP_RNDIS_H_
