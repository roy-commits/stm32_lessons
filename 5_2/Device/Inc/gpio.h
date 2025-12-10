//
// Created by roy on 2025/11/21.
//

#ifndef INC_5_1_THROUGH_BEAM_IR_COUNTER_GPIO_H
#define INC_5_1_THROUGH_BEAM_IR_COUNTER_GPIO_H
#include "stm32f10x.h"

/**
 * @brief GPIO端口-时钟使能宏 映射表（抽象核心）
 * @note 新增端口时，仅需在此处添加一行映射关系，无需修改其他逻辑
 */
typedef struct {
    GPIO_TypeDef *port; // GPIO端口（如GPIOA、GPIOB）
    uint32_t rcc_periph; // 对应时钟使能宏（RCC_APB2Periph_XXX）
} GPIO_PortClockMapTypeDef;

void GPIO_EnableClock(GPIO_TypeDef *port);

#endif //INC_5_1_THROUGH_BEAM_IR_COUNTER_GPIO_H
