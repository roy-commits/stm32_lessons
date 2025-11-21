//
// Created by roy on 2025/11/21.
//
#include "gpio.h"
#include "infrared.h"
#include <stddef.h>


void Infrared_Init(const Infrared_ConfigTypeDef *cfg) {
    if (cfg == NULL) {
        return;
    }

    GPIO_EnableClock(cfg->GPIO_PORT); // 自动使能SCL端口时钟
    GPIO_EnableClock(cfg->AFIO_PORT);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = cfg->GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init((GPIO_TypeDef *) cfg->GPIO_PORT, &GPIO_InitStruct);

    /*AFIO选择中断引脚*/
    GPIO_EXTILineConfig(cfg->GPIO_PortSource, cfg->GPIO_PinSource);

    /**
     * EXTI 初始化
     */
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = cfg->EXTI_Line;          //选择配置外部中断线
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;               //指定外部中断线使能
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;     //指定外部中断线为中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //指定外部中断线为下降沿触发

    EXTI_Init(&EXTI_InitStruct);

    /**
     * NVIC 中断分组
     */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

}

uint16_t Infrared_Read(void) {
    return;
}
