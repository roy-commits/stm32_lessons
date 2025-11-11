//
// Created by roy on 2025/11/6.
//
#include "i2c.h"

// I2C1初始化（OLED通信，100kHz）
void I2C1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;

    // 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    // 配置SCL（PB6）和SDA（PB7）为开漏复用
    GPIO_InitStruct.GPIO_Pin = I2C1_SCL_PIN | I2C1_SDA_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C1_GPIO_PORT, &GPIO_InitStruct);

    // 配置I2C1参数
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct.I2C_ClockSpeed = 100000;  // 100kHz
    I2C_Init(I2C1, &I2C_InitStruct);

    // 使能I2C1
    I2C_Cmd(I2C1, ENABLE);
}

// I2C2初始化（触控板通信，100kHz）
void I2C2_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;

    // 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

    // 配置SCL（PB10）和SDA（PB11）为开漏复用
    GPIO_InitStruct.GPIO_Pin = I2C2_SCL_PIN | I2C2_SDA_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C2_GPIO_PORT, &GPIO_InitStruct);

    // 配置I2C2参数
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct.I2C_ClockSpeed = 100000;  // 100kHz
    I2C_Init(I2C2, &I2C_InitStruct);

    // 使能I2C2
    I2C_Cmd(I2C2, ENABLE);
}

// 发送I2C起始信号
void I2C_Start(I2C_TypeDef* I2Cx) {
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
    I2C_GenerateSTART(I2Cx, ENABLE);
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
}

// 发送设备地址（dir：I2C_Direction_Transmitter/Receiver）
void I2C_SendAddr(I2C_TypeDef* I2Cx, uint8_t addr, uint8_t dir) {
    I2C_Send7bitAddress(I2Cx, addr, dir);
    if (dir == I2C_Direction_Transmitter) {
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    } else {
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    }
}

// 发送1字节数据
void I2C_MySendData(I2C_TypeDef* I2Cx, uint8_t data) {
    I2C_SendData(I2Cx, data);
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

// 接收1字节数据（ack：1-发送ACK，0-发送NACK）
uint8_t I2C_MyReceiveData(I2C_TypeDef* I2Cx, uint8_t ack) {
    I2C_AcknowledgeConfig(I2Cx, ack);  // 配置ACK
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
    return I2C_ReceiveData(I2Cx);
}

// 发送I2C停止信号
void I2C_Stop(I2C_TypeDef* I2Cx) {
    I2C_GenerateSTOP(I2Cx, ENABLE);
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));
}
