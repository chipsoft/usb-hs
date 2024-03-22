#include "bsp_rndis.h"
#include "bsp_usb_hs.h"
#include "tusb.h"
#include "cmsis_os.h"
#include "custom_assert.h"

typedef struct
{
    const drv_usb_hs_t *drv_usb_hs;
} drv_rndis_context_t;

static void task_rndis(const void *pvParameters);
static drv_rndis_status_t drv_rndis_init(const void *hw_context);
static drv_rndis_status_t drv_rndis_deinit(const void *hw_context);

static const drv_rndis_context_t rndis_context = {
    .drv_usb_hs = &drv_usb_hs_0,
};

const drv_rndis_t drv_rndis_0 = {
    .hw_context = &rndis_context,
    .init = &drv_rndis_init,
    .deinit = &drv_rndis_deinit,
};

static drv_rndis_status_t drv_rndis_init(const void *hw_context)
{
    // Run Webserver task
    osThreadDef(TASK_RNDIS, task_rndis, osPriorityNormal, 0, 2048);
    osThreadId task_rndis_handler = osThreadCreate(osThread(TASK_RNDIS), (void *)hw_context);
    CUSTOM_ASSERT(task_rndis_handler != NULL);
    return DRV_RNDIS_STATUS_OK;
}

static drv_rndis_status_t drv_rndis_deinit(const void *hw_context)
{
    return DRV_RNDIS_STATUS_OK;
}

static void task_rndis(const void *pvParameters)
{
    const drv_rndis_context_t *context = (const drv_rndis_context_t *)pvParameters;
    // Low level USB initialization
    hw_usb_hs_init(context->drv_usb_hs);
    tud_init(BOARD_TUD_RHPORT);
    for (;;)
    {
        tud_task();
    }
}
