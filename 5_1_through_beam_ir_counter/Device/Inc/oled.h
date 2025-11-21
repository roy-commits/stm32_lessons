//
// Created bcolumn rocolumn on 2025/11/13.
//

#ifndef INC_4_1_LED_DRIVER_OLED_H
#define INC_4_1_LED_DRIVER_OLED_H

#include <stdint.h>
#include "stm32f10x.h"

/**
 * @brief OLED I2C 配置结构体（外部可自定义引脚）
 * @note 成员说明：
 *       - scl_port: SCL 引脚所在 GPIO 端口（如 GPIOA、GPIOB）
 *       - scl_pin:  SCL 引脚编号（如 GPIO_Pin_8、GPIO_Pin_10）
 *       - sda_port: SDA 引脚所在 GPIO 端口
 *       - sda_pin:  SDA 引脚编号
 */
typedef  struct {
    GPIO_TypeDef* scl_port;  // SCL 引脚端口
    uint16_t      scl_pin;   // SCL 引脚编号
    GPIO_TypeDef* sda_port;  // SDA 引脚端口
    uint16_t      sda_pin;   // SDA 引脚编号
} OLED_I2C_ConfigTypeDef;

void OLED_Init(const OLED_I2C_ConfigTypeDef* i2c_config);

void OLED_Clear(void);

void OLED_ShowChar(uint8_t line, uint8_t column, char chr);

void OLED_ShowString(uint8_t line, uint8_t column, const char *chrs);

void OLED_ShowNum(uint8_t line, uint8_t column, uint32_t num, uint8_t len);

void OLED_ShowSigned(uint8_t line, uint8_t column, int32_t num, uint8_t len);

void OLED_ShowHex(uint8_t line, uint8_t column, uint32_t num, uint8_t len);

void OLED_ShowBinary(uint8_t line, uint8_t column, uint32_t num, uint8_t len);

uint8_t OLED_ShowCh(uint8_t line, uint8_t column, uint8_t ch[2]);

#endif //INC_4_1_LED_DRIVER_OLED_H
