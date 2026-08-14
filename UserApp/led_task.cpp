#include "main.h"
#include "cmsis_os.h"
#include "uart_task.hpp"

extern "C" void led_task(void const *argument)
{
  (void)argument;

  while (1)
  {
    HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin);
    osDelay(led_get_toggle_period_ms());
  }
}