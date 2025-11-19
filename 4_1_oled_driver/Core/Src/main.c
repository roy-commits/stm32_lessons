#include "oled.h"
#include "stm32f10x.h"

int main(void) {
    OLED_I2C_ConfigTypeDef oled_i2c_cfg = {
        GPIOB,
        GPIO_Pin_8,
        GPIOB,
        GPIO_Pin_9
    };

    OLED_Init(&oled_i2c_cfg);

    uint8_t chinese_str1[] = {"中文显示测试"};  // 对应 GB2312 编码：0xD6,0xD0,0xCE,0xC4,0xCF,0xD6,0xCA,0xD3,0xB5,0xCD
    uint8_t chinese_str2[] = {"HZK16 字库集成"};

    // 显示中文字符串
    OLED_ShowCh(1, 1, chinese_str1);  // 第 1 行显示 "中文显示测试"
    OLED_ShowCh(2, 2, chinese_str2);  // 第 2 行显示 "HZK16 字库集成"

    // 混合显示 ASCII 字符串和中文
    OLED_ShowString(3, 1, "Hello World!");      // 第 3 行显示英文

    OLED_ShowNum(4, 1, 2024, 4);                // 第 4 行显示数字

    OLED_ShowCh(4, 6, (uint8_t[]){"年"});   // 第 4 列 6 显示 "年"


    while (1) {
    }
}
