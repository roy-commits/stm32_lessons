cmake_minimum_required(VERSION 3.22)
# 核心配置：项目名称、语言、C标准与构建类型
project(3_2_led_flash C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE) # 支持 clangd 索引
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
message("Build type: ${CMAKE_BUILD_TYPE}")

# 1. STM32 专用配置：宏定义、头文件路径
set(MX_Defines_Syms
        USE_STDPERIPH_DRIVER  # 启用标准外设库
        STM32F10X_MD          # 芯片型号（中容量 F103）
        $<$<CONFIG:Debug>:DEBUG> # Debug 模式下定义 DEBUG 宏
)

set(MX_Include_Dirs
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/ST/STM32F1xx/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F10x_StdPeriph_Driver/Inc
)

# 2. 源文件匹配（精简路径：直接基于主目录，去除冗余 ../ 层级）
# 应用层源文件（业务代码+启动汇编）
file(GLOB_RECURSE MX_Application_Src
        "${CMAKE_CURRENT_SOURCE_DIR}/Core/*.*"  # Core 下所有子目录的 .c 文件
        "${CMAKE_CURRENT_SOURCE_DIR}/startup/*.*" # startup 目录的汇编启动文件
)
# 驱动层源文件（CMSIS+标准外设库）
file(GLOB_RECURSE STM32_Drivers_Src
        "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/*/*.c" # Drivers 下所有子目录的 .c 文件
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

# 4. 主可执行文件构建
add_executable(${PROJECT_NAME})
# 添加应用层源码
target_sources(${PROJECT_NAME} PRIVATE ${MX_Application_Src})
# 链接依赖库（驱动库+接口库）
target_link_libraries(${PROJECT_NAME} PRIVATE STM32_Drivers stm32cubemx)

# 5. 编译与链接优化
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob) # 移除多余 libob.a 依赖
# 构建后清理 map 文件
set_target_properties(${PROJECT_NAME} PROPERTIES
        ADDITIONAL_CLEAN_FILES "${PROJECT_NAME}.map"
)

# 6. 合法性检查：确保 C 标准版本符合要求
if ((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
    message(ERROR "Generated code requires C11 or higher")
endif()