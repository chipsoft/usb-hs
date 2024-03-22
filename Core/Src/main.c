/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
#include "mongoose.h"
#include "printf.h"
#include "bsp_usb_hs.h"
#include "bsp_rndis.h"
#include "tusb.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define UUID ((uint8_t *)UID_BASE) // Unique 96-bit chip ID. TRM 41.1

// Helper macro for MAC generation
#define GENERATE_LOCALLY_ADMINISTERED_MAC()                        \
  {                                                                \
    2, UUID[0] ^ UUID[1], UUID[2] ^ UUID[3], UUID[4] ^ UUID[5],    \
        UUID[6] ^ UUID[7] ^ UUID[8], UUID[9] ^ UUID[10] ^ UUID[11] \
  }
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static struct mg_tcpip_if *s_ifp;
uint8_t tud_network_mac_address[6] = {2, 2, 0x84, 0x6A, 0x96, 0};
static void task_alive(const void *pvParameters);
static void task_web_server(void *param);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
// void cdc_task(void);
bool tud_network_recv_cb(const uint8_t *buf, uint16_t len);
void tud_network_init_cb(void);
uint16_t tud_network_xmit_cb(uint8_t *dst, void *ref, uint16_t arg);
static size_t usb_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp);
static bool usb_up(struct mg_tcpip_if *ifp);
static void fn(struct mg_connection *c, int ev, void *ev_data);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.	 */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  //  MX_USB_OTG_HS_PCD_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  hw_rndis_init(&drv_rndis_0);
  // hw_usb_hs_init(&drv_usb_hs_0);
//  HAL_PCD_MspInit(&hpcd_USB_OTG_HS);
#if CFG_TUSB_OS == OPT_OS_NONE
  // 1ms tick timer
  SysTick_Config(SystemCoreClock / 1000);
#elif CFG_TUSB_OS == OPT_OS_FREERTOS
  // Explicitly disable systick to prevent its ISR runs before scheduler start
  SysTick->CTRL &= ~1U;
#endif

  // For alive task
  osThreadDef(TASK_ALIVE, task_alive, osPriorityNormal, 0, 512);
  osThreadId task_alive_handler = osThreadCreate(osThread(TASK_ALIVE), NULL);
  // For Webserver
  osThreadDef(TASK_WEB_SERVER, task_web_server, osPriorityNormal, 0, 2048);
  osThreadId task_web_server_handler = osThreadCreate(osThread(TASK_WEB_SERVER), NULL);

  osKernelStart();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    //	  tud_task(); // tinyusb device task
    //	  cdc_task();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
uint64_t mg_millis(void)
{                       // Let Mongoose use our uptime function
  return HAL_GetTick(); // Return number of milliseconds since boot
}

// void cdc_task(void)
//{
//   // connected() check for DTR bit
//   // Most but not all terminal client set this when making connection
//   // if ( tud_cdc_connected() )
//   {
//     // connected and there are data available
//     if ( tud_cdc_available() )
//     {
//       // read data
//       char buf[64];
//       uint32_t count = tud_cdc_read(buf, sizeof(buf));
//       (void) count;
//
//       // Echo back
//       // Note: Skip echo by commenting out write() and write_flush()
//       // for throughput test e.g
//       //    $ dd if=/dev/zero of=/dev/ttyACM0 count=10000
//       tud_cdc_write(buf, count);
//       tud_cdc_write_flush();
//     }
//   }
// }

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

static size_t usb_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp)
{
  if (!tud_ready())
    return 0;
  while (!tud_network_can_xmit(len))
    tud_task();
  tud_network_xmit((void *)buf, len);
  (void)ifp;
  return len;
}

static bool usb_up(struct mg_tcpip_if *ifp)
{
  (void)ifp;
  return tud_inited() && tud_ready() && tud_connected();
}

static void fn(struct mg_connection *c, int ev, void *ev_data)
{
  if (ev == MG_EV_HTTP_MSG)
  {
    struct mg_http_message *hm = (struct mg_http_message *)ev_data;
    if (mg_http_match_uri(hm, "/api/debug"))
    {
      int level = mg_json_get_long(hm->body, "$.level", MG_LL_DEBUG);
      mg_log_set(level);
      mg_http_reply(c, 200, "", "Debug level set to %d\n", level);
    }
    else
    {
      mg_http_reply(c, 200, "", "hi\n");
    }
  }
}

static void task_alive(const void *pvParameters)
{
  while (1)
  {
    printf("Blink\r\n");
    HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_SET);
    osDelay(100);
    HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_RESET);
    osDelay(100);
    HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_SET);
    osDelay(100);
    HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13, GPIO_PIN_RESET);
    osDelay(700);
  }
}

static void task_web_server(void *param)
{
  // Mongoose Init
  struct mg_mgr mgr;       // Initialise
  mg_mgr_init(&mgr);       // Mongoose event manager
  mg_log_set(MG_LL_DEBUG); // Set log level
  MG_INFO(("Init TCP/IP stack ..."));
  struct mg_tcpip_driver driver = {.tx = usb_tx, .up = usb_up};
  struct mg_tcpip_if mif = {.mac = GENERATE_LOCALLY_ADMINISTERED_MAC(),
                            .ip = mg_htonl(MG_U32(192, 168, 3, 1)),
                            .mask = mg_htonl(MG_U32(255, 255, 255, 0)),
                            .enable_dhcp_server = true,
                            .driver = &driver,
                            .recv_queue.size = 4096};
  s_ifp = &mif;
  mg_tcpip_init(&mgr, &mif);
  //  mg_timer_add(&mgr, 500, MG_TIMER_REPEAT, blink_cb, &mgr);
  mg_http_listen(&mgr, "tcp://0.0.0.0:80", fn, &mgr);

  MG_INFO(("Init done, starting main loop ..."));
  for (;;)
  {
    mg_mgr_poll(&mgr, 1);
  }
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
