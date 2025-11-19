#include "oled.h"
#include "stm32f10x.h"

int main(void) {

    OLED_Init();

    OLED_ShowChar(1, 1, 'a');

    OLED_ShowString(1, 3, "wawawawawaw");


    while (1) {

    }
}