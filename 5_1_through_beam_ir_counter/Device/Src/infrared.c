//
// Created by roy on 2025/11/21.
//
#include "gpio.h"
#include "infrared.h"
#include <stddef.h>

uint16_t infrared_count;

void Infrared_Init(const Infrared_ConfigTypeDef *cfg) {
    if (cfg == NULL) {
        return;
    }

    GPIO_EnableClock(cfg->GPIO_PORT); // 自动使能SCL端口时钟

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//开启AFIO的时钟，外部中断必须开启AFIO的时钟

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = cfg->GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(cfg->GPIO_PORT, &GPIO_InitStruct);

    /*AFIO选择中断引脚*/
    GPIO_EXTILineConfig(cfg->GPIO_PortSource, cfg->GPIO_PinSource);

    /**
     * EXTI 初始化
     */
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = cfg->EXTI_Line; //选择配置外部中断线
    EXTI_InitStruct.EXTI_LineCmd = ENABLE; //指定外部中断线使能
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; //指定外部中断线为中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //指定外部中断线为下降沿触发

    EXTI_Init(&EXTI_InitStruct);

    /**
     * NVIC 中断分组
     */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn; //选择配置NVIC的EXTI15_10线
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; //指定NVIC线路的抢占优先级为1
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1; //指定NVIC线路的响应优先级为1
    NVIC_Init(&NVIC_InitStruct);
}

uint16_t Infrared_Read(void) {
    return infrared_count;
}

void EXTI15_10_IRQHandler(const Infrared_ConfigTypeDef *cfg) {
    if (EXTI_GetITStatus(cfg->EXTI_Line) == SET) {
        //判断是否是指定外部中断线触发的中断
        /*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
        if (GPIO_ReadInputDataBit((GPIO_TypeDef *) cfg->GPIO_PORT, cfg->GPIO_PIN) == 0) {
            infrared_count++;
        }
        EXTI_ClearITPendingBit(cfg->EXTI_Line); //清除外部中断线的中断标志位
    }
}
