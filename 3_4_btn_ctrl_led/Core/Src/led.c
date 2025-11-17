//
// Created by roy on 2025/11/11.
//
#include "led.h"

void LED_Init(GPIO_TypeDef * gpio_x, uint32_t gpio_pin) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // GPIO init
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = gpio_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(gpio_x, &GPIO_InitStructure);

    GPIO_SetBits(gpio_x, gpio_pin);
}

void LED_ON(GPIO_TypeDef * gpio_x, uint32_t gpio_pin) {
    GPIO_SetBits(gpio_x, gpio_pin);
}

void LED_OFF(GPIO_TypeDef * gpio_x, uint32_t gpio_pin) {
    GPIO_ResetBits(gpio_x, gpio_pin);
}

void LED_Turn(GPIO_TypeDef * gpio_x, uint32_t gpio_pin) {
    if (GPIO_ReadOutputDataBit(gpio_x, gpio_pin) == 0) {
        GPIO_SetBits(gpio_x, gpio_pin);
    } else {
        GPIO_ResetBits(gpio_x, gpio_pin);
    }
}
