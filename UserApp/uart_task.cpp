#include "main.h"
#include "cmsis_os.h"
#include <stdlib.h>
#include <string.h>
#include "uart_task.hpp"

extern UART_HandleTypeDef huart1;

namespace
{
constexpr uint32_t kDefaultTogglePeriodMs = 50U;
constexpr uint32_t kMinTogglePeriodMs = 10U;
constexpr uint32_t kMaxTogglePeriodMs = 60000U;
}

static volatile uint32_t g_led_toggle_period_ms = kDefaultTogglePeriodMs;

extern "C" void led_set_toggle_period_ms(uint32_t period_ms)
{
    if (period_ms < kMinTogglePeriodMs)
    {
        period_ms = kMinTogglePeriodMs;
    }
    else if (period_ms > kMaxTogglePeriodMs)
    {
        period_ms = kMaxTogglePeriodMs;
    }

    g_led_toggle_period_ms = period_ms;
}

extern "C" uint32_t led_get_toggle_period_ms(void)
{
    return g_led_toggle_period_ms;
}

static void uart_send(const char *message)
{
    HAL_UART_Transmit(&huart1,
                      reinterpret_cast<uint8_t *>(const_cast<char *>(message)),
                      static_cast<uint16_t>(strlen(message)),
                      HAL_MAX_DELAY);
}

extern "C" void uart_task(void const *argument)
{
    (void)argument;

    uint8_t received_byte = 0;
    char command[12] = {};
    uint32_t command_length = 0;

    uart_send("\r\n请输入 LED 翻转间隔(ms)，范围 10~60000，例如：500\r\n");

    while (1)
    {
        if (HAL_UART_Receive(&huart1, &received_byte, 1, 20) != HAL_OK)
        {
            continue;
        }

        if (received_byte == '#')
        {
            if (command_length == 0)
            {
                continue;
            }

            command[command_length] = '\0';
            char *end = nullptr;
            unsigned long period = strtoul(command, &end, 10);

            if (*end == '\0' &&
                period >= kMinTogglePeriodMs &&
                period <= kMaxTogglePeriodMs)
            {
                led_set_toggle_period_ms(static_cast<uint32_t>(period));
                uart_send("设置成功\r\n");
            }
            else
            {
                uart_send("输入无效，请输入 10~60000 之间的整数(ms)\r\n");
            }

            command_length = 0;
            command[0] = '\0';
            continue;
        }

        if (received_byte >= '0' && received_byte <= '9')
        {
            if (command_length < sizeof(command) - 1)
            {
                command[command_length++] = static_cast<char>(received_byte);
            }
            else
            {
                command_length = 0;
                command[0] = '\0';
                uart_send("输入过长，请重新输入\r\n");
            }
        }
        else
        {
            command_length = 0;
            command[0] = '\0';
            uart_send("只支持输入数字\r\n");
        }
    }
}