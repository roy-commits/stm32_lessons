//
// Created by roy on 2025/11/13.
//
#include "oled.h"

#include <stddef.h>
#include <string.h>

#include "gpio.h"
#include "oled_font.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

// 静态全局变量：存储外部传入的 I2C 配置（仅驱动内部使用）
static OLED_I2C_ConfigTypeDef g_oled_i2c_config;

/**
 * @brief I2C 引脚电平设置（内部辅助函数，依赖外部配置）
 * @param scl_level: SCL 引脚电平（0=低，1=高）
 * @param sda_level: SDA 引脚电平（0=低，1=高）
 */
static void OLED_I2C_SetPinLevel(uint8_t scl_level, uint8_t sda_level) {
    // 设置 SCL 电平
    GPIO_WriteBit(g_oled_i2c_config.scl_port, g_oled_i2c_config.scl_pin,
                  (BitAction)(scl_level ? Bit_SET : Bit_RESET));
    // 设置 SDA 电平
    GPIO_WriteBit(g_oled_i2c_config.sda_port, g_oled_i2c_config.sda_pin,
                  (BitAction)(sda_level ? Bit_SET : Bit_RESET));
}

/**
 * @brief OLED I2C 初始化（外部传参版）
 * @param config: 指向 I2C 配置结构体的指针（外部定义）
 * @retval None
 * @note 需在外部定义 OLED_I2C_ConfigTypeDef 变量并传入，例：
 *       OLED_I2C_ConfigTypeDef oled_i2c_cfg = {
 *           .scl_port = GPIOB,
 *           .scl_pin  = GPIO_Pin_8,
 *           .sda_port = GPIOB,
 *           .sda_pin  = GPIO_Pin_9
 *       };
 *       OLED_I2C_Init(&oled_i2c_cfg);
 */
void OLED_I2C_Init(const OLED_I2C_ConfigTypeDef* config) {
    if (config == NULL) return;  // 空指针保护

    // 保存配置到静态变量（供内部函数使用）
    g_oled_i2c_config = *config;

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // 开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // -------------------------- 核心优化：抽象化时钟使能 --------------------------
    GPIO_EnableClock(g_oled_i2c_config.scl_port);  // 自动使能SCL端口时钟
    GPIO_EnableClock(g_oled_i2c_config.sda_port);  // 自动使能SDA端口时钟
    // -----------------------------------------------------------------------------

    // 配置 SCL 引脚
    GPIO_InitStructure.GPIO_Pin = g_oled_i2c_config.scl_pin;
    GPIO_Init(g_oled_i2c_config.scl_port, &GPIO_InitStructure);

    // 配置 SDA 引脚
    GPIO_InitStructure.GPIO_Pin = g_oled_i2c_config.sda_pin;
    GPIO_Init(g_oled_i2c_config.sda_port, &GPIO_InitStructure);

    // 初始电平：SCL、SDA 均为高电平
    OLED_I2C_SetPinLevel(1, 1);
}

/**
 * @brief I2C 起始信号（重构后，使用外部配置引脚）
 */
void OLED_I2C_Start(void) {
    OLED_I2C_SetPinLevel(1, 1);  // 总线空闲
    OLED_I2C_SetPinLevel(1, 0);  // SDA 拉低（起始信号：SCL 高时 SDA 下降沿）
    OLED_I2C_SetPinLevel(0, 0);  // SCL 拉低（准备发送数据）
}

/**
 * @brief I2C 停止信号（重构后，使用外部配置引脚）
 */
void OLED_I2C_Stop(void) {
    OLED_I2C_SetPinLevel(0, 0);  // 数据发送完成，SCL 低、SDA 低
    OLED_I2C_SetPinLevel(1, 0);  // SCL 拉高（停止信号：SCL 高时 SDA 上升沿）
    OLED_I2C_SetPinLevel(1, 1);  // SDA 拉高（总线回到空闲状态）
}

/**
 * @brief I2C 发送一个字节（重构后，使用外部配置引脚）
 * @param byte: 要发送的字节（8 位）
 */
void OLED_I2C_Send_Byte(const uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        // 发送当前位（高位在前）
        OLED_I2C_SetPinLevel(0, !!(byte & (0x80 >> i)));  // SCL 低时，SDA 置位当前位
        OLED_I2C_SetPinLevel(1, 1);                      // SCL 拉高，从机采样
        OLED_I2C_SetPinLevel(0, 0);                      // SCL 拉低，准备发送下一位
    }
    // 应答位处理（I2C 标准：从机应答时 SDA 拉低，此处简化处理）
    OLED_I2C_SetPinLevel(1, 1);  // SCL 拉高，等待应答
    OLED_I2C_SetPinLevel(0, 0);  // SCL 拉低，结束应答
}

/**
 * @brief OLED 写入指令（无修改，依赖重构后的 I2C 函数）
 * @param command: 要写入的指令字节
 */
void OLED_Write_Command(const uint8_t command) {
    OLED_I2C_Start();
    OLED_I2C_Send_Byte(0x78);  // OLED I2C 从机地址（默认 0x78，可根据硬件调整）
    OLED_I2C_Send_Byte(0x00);  // 写指令标志（0x00：指令，0x40：数据）
    OLED_I2C_Send_Byte(command);
    OLED_I2C_Stop();
}

/**
 * @brief OLED 写入数据（无修改，依赖重构后的 I2C 函数）
 * @param data: 要写入的数据字节
 */
void OLED_Write_Data(const uint8_t data) {
    OLED_I2C_Start();
    OLED_I2C_Send_Byte(0x78);  // OLED I2C 从机地址
    OLED_I2C_Send_Byte(0x40);  // 写数据标志
    OLED_I2C_Send_Byte(data);
    OLED_I2C_Stop();
}

/**
 * @brief 设置 OLED 光标位置（无修改）
 * @param y: 行坐标（0~7，对应 8 行，每行 8 像素）
 * @param x: 列坐标（0~127，对应 128 列）
 */
void OLED_Cursor(const uint8_t y, const uint8_t x) {
    OLED_Write_Command(0xB0 | y);                  // 设置行地址（0xB0~0xB7 对应 8 行）
    OLED_Write_Command(0x10 | ((x & 0xF0) >> 4));  // 设置列地址高 4 位
    OLED_Write_Command(0x00 | (x & 0x0F));         // 设置列地址低 4 位
}

/**
 * @brief OLED 清屏（无修改）
 */
void OLED_Clear(void) {
    for (uint8_t y = 0; y < 8; y++) {
        OLED_Cursor(y, 0);
        for (uint8_t x = 0; x < 128; x++) {
            OLED_Write_Data(0x00);  // 写入 0x00，清空对应像素
        }
    }
}

/**
 * @brief OLED 显示单个字符（无修改）
 * @param line: 行号（1~4，对应 4 行 16x8 字符）
 * @param column: 列号（1~16，对应 16 列字符）
 * @param chr: 要显示的 ASCII 字符（如 'A'、'1' 等）
 */
void OLED_ShowChar(const uint8_t line, const uint8_t column, const char chr) {
    uint8_t i;
    // 计算光标位置（16x8 字符：每行占 2 个行坐标，每列占 8 个列坐标）
    OLED_Cursor((line - 1) * 2, (column - 1) * 8);
    for (i = 0; i < 8; i++) {
        OLED_Write_Data(asc2_16x8[chr - ' '][i]);  // 显示字符上半部分（8 像素）
    }
    OLED_Cursor((line - 1) * 2 + 1, (column - 1) * 8);
    for (i = 0; i < 8; i++) {
        OLED_Write_Data(asc2_16x8[chr - ' '][i + 8]);  // 显示字符下半部分（8 像素）
    }
}

/**
 * @brief OLED 显示字符串（无修改）
 * @param line: 行号（1~4）
 * @param column: 列号（1~16）
 * @param chrs: 要显示的字符串指针（以 '\0' 结尾）
 */
void OLED_ShowString(const uint8_t line, const uint8_t column, const char *chrs) {
    for (uint8_t i = 0; chrs[i] != '\0'; i++) {
        OLED_ShowChar(line, column + i, chrs[i]);  // 逐个字符显示，列号递增
    }
}

/**
 * @brief 计算 x 的 y 次方（辅助函数，无修改）
 * @param x: 底数
 * @param y: 指数
 * @retval 计算结果（uint32_t 类型）
 */
uint32_t OLED_Pow(uint32_t x, uint32_t y) {
    uint32_t result = 1;
    while (y--) {
        result *= x;
    }
    return result;
}

/**
 * @brief OLED 显示无符号整数（无修改）
 * @param line: 行号（1~4）
 * @param column: 列号（1~16）
 * @param num: 要显示的无符号整数（0~4294967295）
 * @param len: 显示位数（1~10）
 */
void OLED_ShowNum(const uint8_t line, const uint8_t column, const uint32_t num, const uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        // 提取每一位数字（从高位到低位）
        uint8_t digit = num / OLED_Pow(10, len - i - 1) % 10;
        OLED_ShowChar(line, column + i, digit + '0');  // 转换为 ASCII 字符显示
    }
}

/**
 * @brief OLED 显示有符号整数（修复原代码 bug，无修改核心逻辑）
 * @param line: 行号（1~4）
 * @param column: 列号（1~16）
 * @param num: 要显示的有符号整数（-2147483648~2147483647）
 * @param len: 显示位数（1~10，包含符号位）
 */
void OLED_ShowSigned(const uint8_t line, const uint8_t column, const int32_t num, const uint8_t len) {
    int32_t temp = num;
    // 显示符号位
    if (num >= 0) {
        OLED_ShowChar(line, column, '+');
    } else {
        OLED_ShowChar(line, column, '-');
        temp = -num;  // 转为正数处理（注意：-2147483648 转正数会溢出，需根据实际场景优化）
    }
    // 显示数字部分
    for (uint8_t i = 0; i < len - 1; i++) {  // len 包含符号位，数字部分占 len-1 位
        uint8_t digit = temp / OLED_Pow(10, len - i - 2) % 10;
        OLED_ShowChar(line, column + i + 1, digit + '0');
    }
}

/**
 * @brief OLED 显示十六进制数（无修改）
 * @param line: 行号（1~4）
 * @param column: 列号（1~16）
 * @param num: 要显示的十六进制数（0~4294967295）
 * @param len: 显示位数（1~8）
 */
void OLED_ShowHex(const uint8_t line, const uint8_t column, const uint32_t num, const uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        // 提取每一位十六进制数（从高位到低位）
        uint8_t digit = num / OLED_Pow(16, len - i - 1) % 16;
        if (digit < 10) {
            OLED_ShowChar(line, column + i, digit + '0');  // 0~9 转为 ASCII
        } else {
            OLED_ShowChar(line, column + i, digit - 10 + 'A');  // A~F 转为 ASCII
        }
    }
}

/**
 * @brief OLED 显示二进制数（无修改）
 * @param line: 行号（1~4）
 * @param column: 列号（1~16）
 * @param num: 要显示的二进制数（0~4294967295）
 * @param len: 显示位数（1~32）
 */
void OLED_ShowBinary(uint8_t line, uint8_t column, uint32_t num, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        // 提取每一位二进制数（从高位到低位）
        uint8_t bit = num / OLED_Pow(2, len - i - 1) % 2;
        OLED_ShowChar(line, column + i, bit + '0');  // 转为 ASCII 字符显示
    }
}

/**
 * @brief 从 HZK16 字库读取单个汉字的 32 字节点阵数据
 * @param ch: GB2312 双字节编码（ch[0] 高字节，ch[1] 低字节）
 * @param buf: 输出缓冲区（至少 32 字节，存储点阵数据）
 * @retval 0：成功，1：编码非法，2：字库未找到（理论上不会触发）
 */
static uint8_t OLED_HZK16_Read(const uint8_t ch[2], uint8_t buf[32]) {
    // 1. 校验 GB2312 编码合法性（高字节 0xA1~0xF7，低字节 0xA1~0xFE）
    if (ch[0] < 0xA1 || ch[0] > 0xF7 || ch[1] < 0xA1 || ch[1] > 0xFE) {
        memset(buf, 0x00, 32);  // 非法编码返回空数据
        return 1;
    }

    // 2. 计算字库偏移量（GB2312 编码公式：偏移 = (高字节-0xA1)*94 + (低字节-0xA1)）
    uint32_t offset = ((ch[0] - 0xA1) * 94 + (ch[1] - 0xA1)) * 32;

    // 3. 定义 HZK16 字库在 Flash 中的起始地址（与 CMake 中 HZK16_FLASH_ADDR 一致）
#define HZK16_FLASH_BASE 0x08070000
    const uint8_t* hzk16_base = (const uint8_t*)HZK16_FLASH_BASE;

    // 4. 读取点阵数据到缓冲区（直接通过指针访问 Flash 地址）
    memcpy(buf, &hzk16_base[offset], 32);
    return 0;
}

/**
 * @brief OLED 显示单个中文汉字（16×16 点阵，HZK16 字库）
 * @param line: 行号（1~4，每个汉字占 16 行）
 * @param column: 列号（1~8，每个汉字占 16 列）
 * @param ch: 中文双字节编码（GB2312 格式）
 * @retval 0：成功，1：编码非法
 */
uint8_t OLED_ShowCh(const uint8_t line, const uint8_t column, uint8_t ch[2]) {
    uint8_t hzk_buf[32] = {0};
    // 从 HZK16 字库读取点阵数据
    if (OLED_HZK16_Read(ch, hzk_buf) != 0) {
        return 1;  // 编码非法
    }

    // 计算光标位置（与原有逻辑一致）
    const uint8_t x = (column - 1) * 16;  // 中文占 16 列
    const uint8_t y = (line - 1) * 2;     // 中文占 2 个行坐标（y 和 y+1）

    // 显示上半部分（8 行，16 列）
    OLED_Cursor(y, x);
    for (uint8_t i = 0; i < 16; i++) {
        OLED_Write_Data(hzk_buf[i]);
    }

    // 显示下半部分（8 行，16 列）
    OLED_Cursor(y + 1, x);
    for (uint8_t i = 16; i < 32; i++) {
        OLED_Write_Data(hzk_buf[i]);
    }

    return 0;
}

/**
 * @brief OLED 初始化（重构后，需先配置 I2C 引脚）
 * @param i2c_config: OLED I2C 引脚配置结构体指针
 * @retval None
 * @note 必须先调用 OLED_I2C_Init 或直接传入 i2c_config 完成 I2C 初始化
 */
void OLED_Init(const OLED_I2C_ConfigTypeDef* i2c_config) {
    // 延时等待 OLED 硬件复位完成（原逻辑保留）
    for (uint32_t i = 0; i < 1000; i++) {
        for (uint32_t j = 0; j < 1000; j++);
    }

    // 初始化 I2C 引脚（使用外部传入的配置）
    OLED_I2C_Init(i2c_config);

    // OLED 显示初始化指令（原逻辑保留，根据 OLED 手册调整）
    OLED_Write_Command(0xAE);        // 关闭显示
    OLED_Write_Command(0xD5);        // 设置显示时钟分频比/振荡器频率
    OLED_Write_Command(0x80);        // 分频比=1，振荡器频率=默认
    OLED_Write_Command(0xA8);        // 设置多路复用率（0x3F=64 路）
    OLED_Write_Command(0x3F);
    OLED_Write_Command(0xD3);        // 设置显示偏移（0x00=无偏移）
    OLED_Write_Command(0x00);
    OLED_Write_Command(0x40);        // 设置显示开始行（0x40=第 0 行）
    OLED_Write_Command(0xA1);        // 设置左右方向（0xA1=正常，0xA0=反转）
    OLED_Write_Command(0xC8);        // 设置上下方向（0xC8=正常，0xC0=反转）
    OLED_Write_Command(0xDA);        // 设置 COM 引脚硬件配置（0x12=交替 COM 模式）
    OLED_Write_Command(0x12);
    OLED_Write_Command(0x81);        // 设置对比度控制（0xCF=高对比度）
    OLED_Write_Command(0xCF);
    OLED_Write_Command(0xD9);        // 设置预充电周期（0xF1=预充电 15 时钟，放电 1 时钟）
    OLED_Write_Command(0xF1);
    OLED_Write_Command(0xDB);        // 设置 VCOMH 取消选择级别（0x30=0.83V）
    OLED_Write_Command(0x30);
    OLED_Write_Command(0xA4);        // 设置整个显示打开/关闭（0xA4=正常显示，0xA5=全亮）
    OLED_Write_Command(0xA6);        // 设置正常/倒转显示（0xA6=正常，0xA7=倒转）
    OLED_Write_Command(0x8D);        // 设置充电泵（0x14=开启充电泵，0x10=关闭）
    OLED_Write_Command(0x14);
    OLED_Write_Command(0xAF);        // 开启显示

    OLED_Clear();  // 初始化完成后清屏
}
