#include "delay.h"
#include "stm32f10x.h"

int main(void) {

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);       // 开启GPIO时钟

    /*
     * GPIO初始化
     */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        // GPIO模式, 赋值为推挽输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;               // GPIO引脚, 设为GPIO_Pin_0
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // GPIO传输速率

    GPIO_Init(GPIOB, &GPIO_InitStructure);                  // 初始化GPIO

    while (1) {

        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
        Delay_ms(100);
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
        Delay_ms(100);
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
        Delay_ms(100);
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
        Delay_ms(500);

    }
}