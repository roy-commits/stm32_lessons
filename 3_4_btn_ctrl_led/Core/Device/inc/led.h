//
// Created by roy on 2025/11/11.
//
#ifndef INC_3_4_BTN_CTRL_LED_LED_H
#define INC_3_4_BTN_CTRL_LED_LED_H

#include "stm32f10x.h"

typedef struct {
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
} LED_AttrTypeDef;

void LED_Init(const LED_AttrTypeDef *Led_Struct);

void LED_ON(const LED_AttrTypeDef *Led_Struct);

void LED_OFF(const LED_AttrTypeDef *Led_Struct);

void LED_Turn(const LED_AttrTypeDef *Led_Struct);

#endif //INC_3_4_BTN_CTRL_LED_LED_H
