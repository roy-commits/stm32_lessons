//
// Created by roy on 2025/11/6.
//

#ifndef STM32F103C8T6_I2C_H
#define STM32F103C8T6_I2C_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"

// I2C1（OLED）引脚定义
#define I2C1_SCL_PIN    GPIO_Pin_6
#define I2C1_SDA_PIN    GPIO_Pin_7
#define I2C1_GPIO_PORT  GPIOB

// I2C2（触控板）引脚定义
#define I2C2_SCL_PIN    GPIO_Pin_10
#define I2C2_SDA_PIN    GPIO_Pin_11
#define I2C2_GPIO_PORT  GPIOB

void I2C1_Init(void);                     // 初始化I2C1（100kHz，OLED）
void I2C2_Init(void);                     // 初始化I2C2（100kHz，触控板）
void I2C_Start(I2C_TypeDef* I2Cx);        // 发送I2C起始信号
void I2C_SendAddr(I2C_TypeDef* I2Cx, uint8_t addr, uint8_t dir);  // 发送设备地址
void I2C_MySendData(I2C_TypeDef* I2Cx, uint8_t data);  // 发送1字节数据
uint8_t I2C_MyReceiveData(I2C_TypeDef* I2Cx, uint8_t ack);  // 接收1字节数据
void I2C_Stop(I2C_TypeDef* I2Cx);         // 发送I2C停止信号

#endif //STM32F103C8T6_I2C_H