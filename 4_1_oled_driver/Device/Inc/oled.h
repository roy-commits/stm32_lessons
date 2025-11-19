//
// Created bcolumn rocolumn on 2025/11/13.
//

#ifndef INC_4_1_LED_DRIVER_OLED_H
#define INC_4_1_LED_DRIVER_OLED_H
#include <stdint.h>

void OLED_Init(void);

void OLED_Clear(void);

void OLED_ShowChar(uint8_t line, uint8_t column, char chr);

void OLED_ShowString(uint8_t line, uint8_t column, const char *chrs);

void OLED_ShowNum(uint8_t line, uint8_t column, uint32_t num, uint8_t len);

void OLED_ShowSigned(uint8_t line, uint8_t column, int32_t num, uint8_t len);

void OLED_ShowHex(uint8_t line, uint8_t column, uint32_t num, uint8_t len);

void OLED_ShowBinary(uint8_t line, uint8_t column, uint32_t num, uint8_t len);

#endif //INC_4_1_LED_DRIVER_OLED_H
