//
// Created by roy on 2025/11/21.
//

#ifndef INC_5_1_THROUGH_BEAM_IR_COUNTER_INFRARED_H
#define INC_5_1_THROUGH_BEAM_IR_COUNTER_INFRARED_H
#include "stm32f10x.h"

/**
 * @brief Infrared I2C 配置结构体（外部可自定义引脚）
 * @note 成员说明：
 *       - scl_port: SCL 引脚所在 GPIO 端口（如 GPIOA、GPIOB）
 *       - scl_pin:  SCL 引脚编号（如 GPIO_Pin_8、GPIO_Pin_10）
 */
typedef struct {
    uint32_t GPIO_PORT; // GPIO 引脚端口
    uint16_t GPIO_PIN; // GPIO 引脚编号
    uint16_t EXIT_PIN; // 外部EXIT引脚编号
    uint32_t AFIO_PORT; // AFIO 引脚端口
    uint8_t GPIO_PortSource;
    uint8_t GPIO_PinSource;
    uint32_t EXTI_Line; // EXTI line
} Infrared_ConfigTypeDef;

void Infrared_Init(const Infrared_ConfigTypeDef *cfg);

uint16_t Infrared_Read(void);

#endif //INC_5_1_THROUGH_BEAM_IR_COUNTER_INFRARED_H
