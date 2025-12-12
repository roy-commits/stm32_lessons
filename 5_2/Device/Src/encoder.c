//
// Created by roy on 2025/12/10.
//
#include "encoder.h"
#include <stddef.h>
#include "gpio.h"

volatile int16_t Encoder_Count;

void Encoder_Init(Encoder_ConfigTypeDef *cfg) {
    if (cfg == NULL) {
        return;
    }

    GPIO_EnableClock(cfg->GPIO_PORT);
    //开启AFIO的时钟，外部中断必须开启AFIO的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* GPIO初始化 */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = cfg->GPIO_Pin[0] | cfg->GPIO_Pin[1];
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(cfg->GPIO_PORT, &GPIO_InitStruct);

    /* AFIO选择中断引脚 */
    GPIO_EXTILineConfig(cfg->GPIO_PortSource, cfg->GPIO_Pin[0]);
    GPIO_EXTILineConfig(cfg->GPIO_PortSource, cfg->GPIO_Pin[1]);

    /*EXIT初始化*/
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = cfg->EXTI_Line[0] | cfg->EXTI_Line[1];
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStruct);

    /*NVIC中断分组*/
    NVIC_PriorityGroupConfig(cfg->NVIC_PriorityGroup);

    /*NVIC配置*/
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStruct);
}

int16_t Encoder_Read(void) {
    int16_t _value = Encoder_Count;
    Encoder_Count = 0;
    return _value;
}

void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0) == SET) {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) {
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) {
                Encoder_Count--;
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line1) == SET) {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) {
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2) == 0) {
                Encoder_Count++;
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}
