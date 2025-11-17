//
// Created by roy on 2025/11/11.
//
#ifndef INC_3_4_BTN_CTRL_LED_LED_H
#define INC_3_4_BTN_CTRL_LED_LED_H

#include "stm32f10x.h"

void LED_Init(GPIO_TypeDef * gpio_x, uint32_t gpio_pin);

void LED_ON(GPIO_TypeDef * gpio_x, uint32_t gpio_pin);

void LED_OFF(GPIO_TypeDef * gpio_x, uint32_t gpio_pin);

void LED_Turn(GPIO_TypeDef * gpio_x, uint32_t gpio_pin);

#endif //INC_3_4_BTN_CTRL_LED_LED_H
