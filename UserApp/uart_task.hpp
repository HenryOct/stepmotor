#ifndef UART_TASK_HPP
#define UART_TASK_HPP

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void led_set_toggle_period_ms(uint32_t period_ms);
uint32_t led_get_toggle_period_ms(void);

#ifdef __cplusplus
}
#endif

#endif
