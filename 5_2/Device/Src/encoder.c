//
// Created by roy on 2025/12/10.
//
#include "encoder.h"

#include <stddef.h>

#include "gpio.h"

void encoder_init(Encoder_ConfigTypeDef *cfg) {
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


}

int16_t encoder_read(void) {
    return 0;
}
