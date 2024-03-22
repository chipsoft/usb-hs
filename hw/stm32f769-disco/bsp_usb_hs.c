#include "bsp_usb_hs.h"
#include "FreeRTOS.h"
#include "usb_otg.h"
#include "gpio.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_rcc.h"
#include "stm32f7xx_hal_rcc_ex.h"

typedef struct
{

} drv_usb_hs_context_t;

static drv_usb_hs_status_t drv_usb_hs_init(const void *hw_context);
static drv_usb_hs_status_t drv_usb_hs_deinit(const void *hw_context);

static const drv_usb_hs_context_t drv_usb_hs_context_0 = {

};

const drv_usb_hs_t drv_usb_hs_0 = {
    .init = drv_usb_hs_init,
    .deinit = drv_usb_hs_deinit,
};

const drv_usb_hs_t *drv_api_array[DRV_USB_HS_COUNT] = {
    &drv_usb_hs_0,
};

static drv_usb_hs_status_t drv_usb_hs_init(const void *hw_context)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USB_OTG_HS GPIO Configuration
    PB5     ------> USB_OTG_HS_ULPI_D7
    PI11     ------> USB_OTG_HS_ULPI_DIR
    PH4     ------> USB_OTG_HS_ULPI_NXT
    PB13     ------> USB_OTG_HS_ULPI_D6
    PB12     ------> USB_OTG_HS_ULPI_D5
    PC0     ------> USB_OTG_HS_ULPI_STP
    PA5     ------> USB_OTG_HS_ULPI_CK
    PB10     ------> USB_OTG_HS_ULPI_D3
    PA3     ------> USB_OTG_HS_ULPI_D0
    PB1     ------> USB_OTG_HS_ULPI_D2
    PB0     ------> USB_OTG_HS_ULPI_D1
    PB11     ------> USB_OTG_HS_ULPI_D4
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_13 | GPIO_PIN_12 | GPIO_PIN_10 | GPIO_PIN_1 | GPIO_PIN_0 | GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USB_OTG_HS clock enable */
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
    __HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE();

    // If freeRTOS is used, IRQ priority is limit by max syscall ( smaller is higher )
    NVIC_SetPriority(OTG_HS_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
    return DRV_USB_HS_STATUS_OK;
}

static drv_usb_hs_status_t drv_usb_hs_deinit(const void *hw_context)
{
    return DRV_USB_HS_STATUS_OK;
}
