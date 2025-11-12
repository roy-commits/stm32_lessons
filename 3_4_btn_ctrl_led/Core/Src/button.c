//
// Created by roy on 2025/11/11.
//
#include "button.h"

#include "delay.h"
#include "stm32f10x_rcc.h"

/**
 * @brief 按键初始化
 *
 * @param None
 *
 * @retval None
 */
void Button_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
 * @brief 获取键位码
 *
 * @param None
 *
 * @retval 按钮键码值, 范围0～2, 返回0表示没有按钮按下
 */
uint8_t Button_GetNum(void) {
    uint8_t Button_Number = 0; // 默认按钮键码值为0

    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) {
        // 获取PB1输入寄存器的值，若为0则表示按键1被按下
        Delay_ms(20); // 延时消抖
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) {
        } // 等待按钮释放
        Delay_ms(20); // 延时消抖
        Button_Number = 1; // 取键位值为1
    }

    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0) {
        // 获取PB11输入寄存器的值，若为0则表示按键2被按下
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0) {
        }
        Delay_ms(20);
        Button_Number = 2;
    }

    return Button_Number;
}
