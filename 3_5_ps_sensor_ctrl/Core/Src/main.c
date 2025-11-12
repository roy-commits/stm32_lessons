#include "buzzer.h"
#include "ps_sensor.h"
#include "stm32f10x.h"

uint8_t Button_Number;

int main(void) {

    Buzzer_Init();
    PS_Sensor_Init();

    while (1) {
        if (PS_Sensor_GET() == 1) {
            Buzzer_On();
        } else {
            Buzzer_Off();
        }
    }
}
