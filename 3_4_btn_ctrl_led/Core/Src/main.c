#include "button.h"
#include "delay.h"
#include "led.h"
#include "stm32f10x.h"

uint8_t Button_Number;

int main(void) {
    const LED_AttrTypeDef Led_Struct_1 = {GPIOA,GPIO_Pin_1};
    const LED_AttrTypeDef Led_Struct_2 = {GPIOA,GPIO_Pin_2};

    LED_Init(&Led_Struct_1); // LED初始化
    LED_Init(&Led_Struct_2); // LED初始化
    Button_Init(); // 按键初始化

    while (1) {
        Button_Number = Button_GetNum();

        if (Button_Number == 1) {
            LED_Turn(&Led_Struct_1);
        }

        if (Button_Number == 2) {
            LED_Turn(&Led_Struct_2);
        }
    }
}
