//
// Created by roy on 2025/11/21.
//
#include "gpio.h"

#include <stddef.h>

// 端口-时钟映射表（支持STM32F1全系列常用端口，可按需扩展）
static const GPIO_PortClockMapTypeDef g_gpioPortClockMap[] = {
    {GPIOA, RCC_APB2Periph_GPIOA},
    {GPIOB, RCC_APB2Periph_GPIOB},
    {GPIOC, RCC_APB2Periph_GPIOC},
    {GPIOD, RCC_APB2Periph_GPIOD},
    {GPIOE, RCC_APB2Periph_GPIOE},
    {GPIOF, RCC_APB2Periph_GPIOF},
    {GPIOG, RCC_APB2Periph_GPIOG},
};

void GPIO_EnableClock(GPIO_TypeDef *port) {
    // 遍历映射表，查找端口对应的时钟使能宏
    uint8_t _map_size = sizeof(g_gpioPortClockMap) / sizeof(GPIO_PortClockMapTypeDef);
    for (uint8_t i = 0; i < _map_size; i++) {
        if (g_gpioPortClockMap[i].port == port) {
            // 找到对应时钟，使能端口时钟
            RCC_APB2PeriphClockCmd(g_gpioPortClockMap[i].rcc_periph, ENABLE);
            break; // 找到后退出循环，避免冗余遍历
        }
    }
}
