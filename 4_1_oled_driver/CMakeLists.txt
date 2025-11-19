cmake_minimum_required(VERSION 3.22)
# 核心配置：项目名称、语言、C标准与构建类型
project(4_1_led_driver C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE) # 支持 clangd 索引
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif ()
message("Build type: ${CMAKE_BUILD_TYPE}")

# 1. STM32 专用配置：宏定义、头文件路径
set(MX_Defines_Syms
        USE_STDPERIPH_DRIVER  # 启用标准外设库
        STM32F10X_MD          # 芯片型号（中容量 F103，如 STM32F103C8T6）
        $<$<CONFIG:Debug>:DEBUG> # Debug 模式下定义 DEBUG 宏
)

set(MX_Include_Dirs
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Device/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/ST/STM32F1xx/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Inc
)

# 2. 源文件匹配（优化：仅保留 .c 和 .s 文件，避免非代码文件干扰）
# 应用层源文件（业务代码+启动汇编）
file(GLOB_RECURSE MX_Application_Src
        "${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/*.c"  # Core 下的 C 代码
        "${CMAKE_CURRENT_SOURCE_DIR}/Device/Src/*.c"  # Device 下的 C 代码
        "${CMAKE_CURRENT_SOURCE_DIR}/startup/*.s"     # startup 目录的汇编启动文件
)
# 驱动层源文件（CMSIS+标准外设库）
file(GLOB_RECURSE STM32_Drivers_Src
        "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/*/*.c" # Drivers 下所有子目录的 C 代码
)

# 3. 库目标构建（接口库+驱动库，解耦依赖）
# 接口库：统一管理头文件和宏定义
add_library(stm32cubemx INTERFACE)
target_include_directories(stm32cubemx INTERFACE ${MX_Include_Dirs})
target_compile_definitions(stm32cubemx INTERFACE ${MX_Defines_Syms})

# 驱动库：编译外设库源码（OBJECT 库避免重复编译）
add_library(STM32_Drivers OBJECT)
target_sources(STM32_Drivers PRIVATE ${STM32_Drivers_Src})
target_link_libraries(STM32_Drivers PUBLIC stm32cubemx)

# 4. 主可执行文件构建（关键：先定义目标，再进行链接）
add_executable(${PROJECT_NAME})
# 添加应用层源码
target_sources(${PROJECT_NAME} PRIVATE ${MX_Application_Src})
# 链接依赖库（驱动库+接口库）
target_link_libraries(${PROJECT_NAME} PRIVATE STM32_Drivers stm32cubemx)

# 5. HZK16 字库集成（修正顺序：在主目标定义后进行链接）
# 定义 HZK16 字库路径和 Flash 烧写地址（关键修正：STM32F103C8T6 仅 64KB Flash，需调整地址）
# 64KB Flash 地址范围：0x08000000 ~ 0x08010000（共 65536 字节）
# HZK16.bin 大小约 135KB，64KB Flash 无法容纳！建议更换芯片（如 STM32F103RCT6，256KB Flash）
# 若坚持使用 C8T6，需裁剪字库（仅保留少量汉字），以下地址为示例（需根据实际字库大小调整）
set(HZK16_BIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/HZK16.bin)
set(HZK16_FLASH_ADDR 0x0800C000)  # 64KB Flash 下，预留前 48KB（0x08000000~0x0800BFFF）给程序

# 将字库文件转换为目标文件（.o），指定存储段为 "HZK16"
add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/HZK16.o
        COMMAND ${CMAKE_OBJCOPY} -I binary -O elf32-littlearm -B arm --rename-section .data=.HZK16 ${HZK16_BIN_PATH} ${CMAKE_BINARY_DIR}/HZK16.o
        DEPENDS ${HZK16_BIN_PATH}
        COMMENT "Converting HZK16.bin to object file"
)

# 创建静态库，链接字库目标文件
add_library(HZK16_LIB STATIC ${CMAKE_BINARY_DIR}/HZK16.o)

set_target_properties(HZK16_LIB PROPERTIES LINKER_LANGUAGE C)

# 链接字库到主项目，指定存储段
target_link_libraries(${PROJECT_NAME} PRIVATE HZK16_LIB)
set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "-Wl,--section-start=.HZK16=${HZK16_FLASH_ADDR}"
)

# 6. 编译与链接优化
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob) # 移除多余 libob.a 依赖
# 构建后清理 map 文件
set_target_properties(${PROJECT_NAME} PROPERTIES
        ADDITIONAL_CLEAN_FILES "${PROJECT_NAME}.map"
)

# 7. 合法性检查：确保 C 标准版本符合要求
if ((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
    message(ERROR "Generated code requires C11 or higher")
endif ()

# 8. 生成二进制文件和 hex 文件（方便烧写）
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        # 生成 .bin 文件
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
        # 生成 .hex 文件
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
        COMMENT "Generated binary and hex files: ${PROJECT_NAME}.bin, ${PROJECT_NAME}.hex"
)