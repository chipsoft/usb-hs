#include "FreeRTOS.h"
#include "task.h"
#include "stm32f7xx.h"

uint32_t HAL_GetTick(void)
{
  return xTaskGetTickCount();
}

void vApplicationMallocFailedHook(void)
{
	configASSERT(0);
}

void vApplicationStackOverflowHook(xTaskHandle pxTask, char *pcTaskName)
{
  (void) pxTask;
  (void) pcTaskName;
  configASSERT(0);
}

void vApplicationIdleHook(void)
{
	SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
	/* Request Wait For Interrupt */
	__WFI();
}
