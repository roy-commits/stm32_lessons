#include "infrared.h"
#include "oled.h"
#include "stm32f10x.h"

int main(void) {
    Infrared_ConfigTypeDef Infrared_Config = {
        GPIOB,
        GPIO_Pin_14,
        GPIO_PortSourceGPIOB,
        GPIO_PinSource14,
        EXTI_Line14
    };

    OLED_I2C_ConfigTypeDef OLED_Config = {
        GPIOB,
        GPIO_Pin_8,
        GPIOB,
        GPIO_Pin_9
    };

    OLED_Init(&OLED_Config);

    Infrared_Init(&Infrared_Config);

    while (1) {
    }
}
