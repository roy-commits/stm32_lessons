//
// Created by roy on 2025/12/10.
//

#ifndef INC_5_2_ENCODER_H
#define INC_5_2_ENCODER_H
#include <stdint.h>

#include "stm32f10x.h"

typedef struct {
    GPIO_TypeDef *GPIO_PORT;
    uint16_t GPIO_Pin[2];
    uint8_t GPIO_PortSource;
    uint32_t GPIO_PinSource[2];
    uint32_t EXTI_Line[2];
    uint32_t NVIC_PriorityGroup;
} Encoder_ConfigTypeDef;

void Encoder_Init(Encoder_ConfigTypeDef *encoder_config);

int16_t Encoder_Read(void);

#endif //INC_5_2_ENCODER_H
