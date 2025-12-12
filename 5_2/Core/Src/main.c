#include "encoder.h"
#include "oled.h"
#include "stm32f10x.h"

volatile int16_t Num;

int main(void) {
    OLED_Init();

    Encoder_ConfigTypeDef encoder_config = {
        .GPIO_PORT = GPIOB,
        .GPIO_Pin = {GPIO_Pin_0, GPIO_Pin_1},
        .GPIO_PortSource = GPIO_PortSourceGPIOB,
        .GPIO_PinSource = {GPIO_PinSource0, GPIO_PinSource1},
        .EXTI_Line = {EXTI_Line0, EXTI_Line1},
        .NVIC_PriorityGroup = NVIC_PriorityGroup_2
    };
    Encoder_Init(&encoder_config);

    OLED_ShowString(1, 1, "Num:");

    while (1) {
        const int16_t _delta = Encoder_Read();
        Num = (int16_t) (Num + _delta); // 直接显式转换，消除警告
        OLED_ShowSignedNum(1, 1, Num, 5);
    }
}
