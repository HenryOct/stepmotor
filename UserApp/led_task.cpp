#include "FreeRTOS.h"
#include "main.h"
#include "stm32f1xx_hal.h"

extern "C" void led_task()
{
  while (1)
  {
    HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin);
    HAL_Delay(100);
  }
}