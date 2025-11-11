//
// Created by roy on 2025/11/11.
//
#include "led.h"
#include <stddef.h>

void LED_Init(const LED_AttrTypeDef *Led_Struct) {
    if (Led_Struct == NULL) return;

    RCC_APB2PeriphClockCmd(Led_Struct->GPIO_Pin, ENABLE);

    // GPIO init
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = Led_Struct->GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Led_Struct->GPIOx, &GPIO_InitStructure);

    GPIO_SetBits(Led_Struct->GPIOx, Led_Struct->GPIO_Pin);
}

void LED_ON(const LED_AttrTypeDef *Led_Struct) {
    if (Led_Struct == NULL) return;
    GPIO_SetBits(Led_Struct->GPIOx, Led_Struct->GPIO_Pin);
}

void LED_OFF(const LED_AttrTypeDef *Led_Struct) {
    if (Led_Struct == NULL) return;
    GPIO_ResetBits(Led_Struct->GPIOx, Led_Struct->GPIO_Pin);
}

void LED_Turn(const LED_AttrTypeDef *Led_Struct) {
    if (Led_Struct == NULL) return;
    if (GPIO_ReadOutputDataBit(Led_Struct->GPIOx, Led_Struct->GPIO_Pin) == 0) {
        GPIO_SetBits(Led_Struct->GPIOx, Led_Struct->GPIO_Pin);
    } else {
        GPIO_ResetBits(Led_Struct->GPIOx, Led_Struct->GPIO_Pin);
    }
}
