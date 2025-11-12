#include "delay.h"
#include "stm32f10x.h"

int main(void) {

    /*
     * 开启时钟
     */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /*
     * GPIO初始化
     */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        // GPIO模式, 赋值为推挽输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;               // GPIO引脚, 设为GPIO_Pin_0
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // GPIO传输速率

    GPIO_Init(GPIOA, &GPIO_InitStructure);                  // 初始化GPIO

    while (1) {
        // 设置GPIOA引脚数据
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
        Delay_ms(500);
        GPIO_SetBits(GPIOA, GPIO_Pin_0);
        Delay_ms(500);

        GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
        Delay_ms(500);
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
        Delay_ms(500);

        GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)0);
        Delay_ms(500);
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)1);
        Delay_ms(500);
    }
}