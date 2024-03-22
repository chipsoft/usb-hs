#include <stdint.h>
#include <stdio.h>
#include "usart.h"

extern UART_HandleTypeDef huart1;

int fputc(int ch, FILE *stream)
{
   // Transmit data
   (void)stream; // Avoid compiler warning (unused parameter
   uint8_t c = (uint8_t)ch;
   HAL_UART_Transmit(&huart1, &c, sizeof(char), HAL_MAX_DELAY);
   //On success, the character written is returned
   return ch;
}

void _putchar(char ch) // Send char to debug log
{
	uint8_t c = (uint8_t)ch;
	HAL_UART_Transmit(&huart1, &c, sizeof(char), HAL_MAX_DELAY);
}

int _write(int file, char *ptr, int len) 
{
   if (file == 1) 
   {
      for(int i = 0; i < len; i++)
      {
    	  HAL_UART_Transmit(&huart1, (uint8_t*)ptr + i, sizeof(char), HAL_MAX_DELAY);
      }
   }
   return len;
}

