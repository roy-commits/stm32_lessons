//
// Created by roy on 2025/11/13.
//
#include "oled.h"
#include "oled_font.h"

#include "stm32f10x_rcc.h"

#define OLED_W_SCL(x)       GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction) (x))
#define OLED_W_SDA(x)       GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction) (x))

void OLED_I2C_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

void OLED_I2C_Start(void) {
    OLED_W_SDA(1);
    OLED_W_SCL(1);
    OLED_W_SDA(0);
    OLED_W_SCL(0);
}

void OLED_I2C_Stop(void) {
    OLED_W_SDA(0);
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

/**
 * @brief 通过I2C发送一个字节
 * @param byte 要发送的字节
 * @retval None
 */
void OLED_I2C_Send_Byte(const uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        OLED_W_SDA(!!(byte & (0x80 >> i)));
        OLED_W_SCL(1);
        OLED_W_SCL(0);
    }
    OLED_W_SCL(1);
    OLED_W_SCL(0);
}

/**
 * @brief oled指令写入
 * @param command 要写入的指令
 * @retval None
 */
void OLED_Write_Command(const uint8_t command) {
    OLED_I2C_Start();
    OLED_I2C_Send_Byte(0x78); // 从机地址
    OLED_I2C_Send_Byte(0x00); //写命令
    OLED_I2C_Send_Byte(command);
    OLED_I2C_Stop();
}

/**
 * @brief oled数据写入
 * @param data 要写入的数据
 * @retval None
 */
void OLED_Write_Data(const uint8_t data) {
    OLED_I2C_Start();
    OLED_I2C_Send_Byte(0x78);
    OLED_I2C_Send_Byte(0x40);
    OLED_I2C_Send_Byte(data);
    OLED_I2C_Stop();
}

/**
 * @brief 设置oled光标位置
 * @param y 以左上角为起点, 向下的坐标, 取值范围: 0～7
 * @param x 从左上角为起点, 向右的坐标, 取值范围: 0～127
 * @retval None
 */
void OLED_Cursor(const uint8_t y, const uint8_t x) {
    OLED_Write_Command(0xB0 | y); //设置行坐标
    OLED_Write_Command(0x10 | ((x & 0xF0) >> 4)); //设置列坐标(高4位)
    OLED_Write_Command(0x00 | (x & 0x0F)); //设置列坐标(低4位)
}

/**
 * @brief oled数据清除
 * @retval None
 */
void OLED_Clear(void) {
    for (uint8_t y = 0; y < 8; y++) {
        OLED_Cursor(y, 0);
        for (uint8_t x = 0; x < 128; x++) {
            OLED_Write_Data(0x00);
        }
    }
}

/**
 * @brief oled屏幕显示字符
 * @param line 行, 范围1～4
 * @param column 列, 范围1～16
 * @param chr 显示字符, 范围@ref asc2_16x8
 *
 */
void OLED_ShowChar(const uint8_t line, const uint8_t column, const char chr) {
    uint8_t i;
    OLED_Cursor((line - 1) * 2, (column - 1) * 8); //设置光标位置在上半部分
    for (i = 0; i < 8; i++) {
        OLED_Write_Data(asc2_16x8[chr - ' '][i]); //显示上半部分内容
    }
    OLED_Cursor((line - 1) * 2 + 1, (column - 1) * 8); //设置光标下半部分
    for (i = 0; i < 8; i++) {
        OLED_Write_Data(asc2_16x8[chr - ' '][i + 8]); //显示下半部分内容
    }
}

/**
 * @brief oled屏幕显示字符串
 * @param line 行, 范围1～4
 * @param column 列, 范围1～16
 * @param chrs 字符串, 范围@ref asc2_16x8
 */
void OLED_ShowString(const uint8_t line, const uint8_t column, const char *chrs) {
    for (uint8_t i = 0; chrs[i] != '\0'; i++) {
        OLED_ShowChar(line, column, chrs[i]);
    }
}

uint32_t OLED_Pow(uint32_t x, uint32_t y) {
    uint32_t result = 1;
    while (y--) {
        result *= x;
    }
    return result;
}

void OLED_ShowNum(const uint8_t line, const uint8_t column, const uint32_t num, const uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        OLED_ShowChar(line, column + 1, num / OLED_Pow(10, len - i - 1) % 10 + '0');
    }
}

void OLED_ShowSigned(const uint8_t line, const uint8_t column, const int32_t num, const uint8_t len) {
    uint8_t i;
    int32_t temp;
    if (num >= 0) {
        OLED_ShowChar(line, column, '+');
        temp = num;
    } else {
        OLED_ShowChar(line, column, '-');
        temp = -num;
    }

    for (uint8_t i = 0; i < len; i++) {
        OLED_ShowChar(line, column, temp / OLED_Pow(10, len - i - 1 % 10) + '0');
    }
}

void OLED_ShowHex(const uint8_t line, const uint8_t column, const uint32_t num, const uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        const uint8_t temp = num / OLED_Pow(16, len - i - 1) % 16;
        if (temp < 10) {
            OLED_ShowChar(line, column + i, temp + '0');
        } else {
            OLED_ShowChar(line, column + i, temp - 10 + 'A');
        }
    }
}

void OLED_ShowBinary(uint8_t line, uint8_t column, uint32_t num, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        OLED_ShowChar(line, column, num / OLED_Pow(2, len - i - 1) % 2 + '0');
    }
}

void OLED_Init(void) {
    for (uint32_t i = 0; i < 1000; i++) {
        for (uint32_t j = 0; j < 1000; j++);
    }

    OLED_I2C_Init(); //端口初始化

    OLED_Write_Command(0xAE); //关闭显示

    OLED_Write_Command(0xD5); //设置显示时钟分频比/振荡器频率
    OLED_Write_Command(0x80);

    OLED_Write_Command(0xA8); //设置多路复用率
    OLED_Write_Command(0x3F);

    OLED_Write_Command(0xD3); //设置显示偏移
    OLED_Write_Command(0x00);

    OLED_Write_Command(0x40); //设置显示开始行

    OLED_Write_Command(0xA1); //设置左右方向，0xA1正常 0xA0左右反置

    OLED_Write_Command(0xC8); //设置上下方向，0xC8正常 0xC0上下反置

    OLED_Write_Command(0xDA); //设置COM引脚硬件配置
    OLED_Write_Command(0x12);

    OLED_Write_Command(0x81); //设置对比度控制
    OLED_Write_Command(0xCF);

    OLED_Write_Command(0xD9); //设置预充电周期
    OLED_Write_Command(0xF1);

    OLED_Write_Command(0xDB); //设置VCOMH取消选择级别
    OLED_Write_Command(0x30);

    OLED_Write_Command(0xA4); //设置整个显示打开/关闭

    OLED_Write_Command(0xA6); //设置正常/倒转显示

    OLED_Write_Command(0x8D); //设置充电泵
    OLED_Write_Command(0x14);

    OLED_Write_Command(0xAF); //开启显示

    OLED_Clear(); //OLED清屏
}
