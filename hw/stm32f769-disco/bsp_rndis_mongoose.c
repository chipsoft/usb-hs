#include "bsp_rndis_mongoose.h"
#include "bsp_usb_hs.h"
#include "tusb.h"
#include "cmsis_os.h"
#include "custom_assert.h"
#include "mongoose.h"

extern struct mg_tcpip_if *s_ifp;

typedef struct
{
    const drv_usb_hs_t *drv_usb_hs;
} drv_rndis_context_t;

static void task_rndis(const void *pvParameters);
static drv_rndis_status_t drv_rndis_init(const void *hw_context);
static drv_rndis_status_t drv_rndis_deinit(const void *hw_context);
static drv_rndis_status_t drv_rndis_tx(const void *hw_context, const void *buf, size_t len, size_t *written);
static drv_rndis_status_t drv_rndis_up(const void *hw_context, bool *is_up);

static const drv_rndis_context_t rndis_context = {
    .drv_usb_hs = &drv_usb_hs_0,
};

const drv_rndis_t drv_rndis_0 = {
    .hw_context = &rndis_context,
    .init = &drv_rndis_init,
    .deinit = &drv_rndis_deinit,
    .tx = &drv_rndis_tx,
    .up = &drv_rndis_up,
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

static drv_rndis_status_t drv_rndis_tx(const void *hw_context, const void *buf, size_t len, size_t *written)
{
    CUSTOM_ASSERT(hw_context != NULL);
    CUSTOM_ASSERT(buf != NULL);
    CUSTOM_ASSERT(written != NULL);
    if (!tud_ready())
    {
        *written = 0;
        return DRV_RNDIS_STATUS_ERROR;
    }
    while (!tud_network_can_xmit(len))
    {
        tud_task();
    }
    tud_network_xmit((void *)buf, len);
    *written = len;
    return DRV_RNDIS_STATUS_OK;
}

static drv_rndis_status_t drv_rndis_up(const void *hw_context, bool *is_up)
{
    CUSTOM_ASSERT(hw_context != NULL);
    CUSTOM_ASSERT(is_up != NULL);
    *is_up = tud_inited() && tud_ready() && tud_connected();
    return DRV_RNDIS_STATUS_OK;
}

// This is internal callback function for TinyUSB & Mongoose
void tud_network_init_cb(void)
{
}

uint16_t tud_network_xmit_cb(uint8_t *dst, void *ref, uint16_t arg)
{
  // MG_INFO(("SEND %hu", arg));
  memcpy(dst, ref, arg);
  return arg;
}

bool tud_network_recv_cb(const uint8_t *buf, uint16_t len)
{
  mg_tcpip_qwrite((void *)buf, len, s_ifp);
  // MG_INFO(("RECV %hu", len));
  // mg_hexdump(buf, len);
  tud_network_recv_renew();
  return true;
}

// Driver function for Mongoose & RNDIS
size_t usb_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp)
{
  if (!tud_ready())
    return 0;
  while (!tud_network_can_xmit(len))
    tud_task();
  tud_network_xmit((void *)buf, len);
  (void)ifp;
  return len;
}

bool usb_up(struct mg_tcpip_if *ifp)
{
  (void)ifp;
  return tud_inited() && tud_ready() && tud_connected();
}

