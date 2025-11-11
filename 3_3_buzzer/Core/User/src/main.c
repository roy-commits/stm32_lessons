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
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;               // GPIO引脚, 设为GPIO_Pin_0
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // GPIO传输速率

    GPIO_Init(GPIOA, &GPIO_InitStructure);                  // 初始化GPIO

    while (1) {

        /*使用GPIO_Write，同时设置GPIOA所有引脚的高低电平，实现LED流水灯*/
        GPIO_Write(GPIOA, ~0x0001);
        Delay_ms(500);
        GPIO_Write(GPIOA,~0x0002);
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0004);
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0008);
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0010);
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0020);
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0040);
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0080);
        Delay_ms(500);

    }
}