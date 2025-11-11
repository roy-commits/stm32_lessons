cmake_minimum_required(VERSION 3.22)
# Enable CMake support for ASM and C languages
enable_language(C ASM)
# STM32CubeMX generated symbols (macros)
set(MX_Defines_Syms
        USE_STDPERIPH_DRIVER
        STM32F10X_MD
        $<$<CONFIG:Debug>:DEBUG>
)

# STM32CubeMX generated include paths
set(MX_Include_Dirs
        ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Device/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../Core/User/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../Core/System/inc

        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/CMSIS/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/inc
)

# STM32CubeMX generated application sources
set(MX_Application_Src

        ${CMAKE_CURRENT_SOURCE_DIR}/../Core/System/src/delay.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Core/System/src/syscalls.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Core/System/src/sysmem.c

        ${CMAKE_CURRENT_SOURCE_DIR}/../Core/User/src/i2c.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Core/User/src/main.c

        ${CMAKE_CURRENT_SOURCE_DIR}/../startup_stm32f103xb.s
)

# STM32 HAL/LL Drivers
set(STM32_Drivers_Src
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/CMSIS/src/core_cm3.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/CMSIS/src/system_stm32f10x.c

        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/misc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_adc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_bkp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_can.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_cec.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_crc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_dac.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_dbgmcu.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_dma.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_flash.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_fsmc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_i2c.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_it.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_iwdg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_pwr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_rtc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_sdio.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/STM32F10x_StdPeriph_Driver/src/stm32f10x_wwdg.c

)

# Drivers Midllewares


# Link directories setup
set(MX_LINK_DIRS

)
# Project static libraries
set(MX_LINK_LIBS
        STM32_Drivers
        ${TOOLCHAIN_LINK_LIBRARIES}

)
# Interface library for includes and symbols
add_library(stm32cubemx INTERFACE)
target_include_directories(stm32cubemx INTERFACE ${MX_Include_Dirs})
target_compile_definitions(stm32cubemx INTERFACE ${MX_Defines_Syms})

# Create STM32_Drivers static library
add_library(STM32_Drivers OBJECT)
target_sources(STM32_Drivers PRIVATE ${STM32_Drivers_Src})
target_link_libraries(STM32_Drivers PUBLIC stm32cubemx)


# Add STM32CubeMX generated application sources to the project
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${MX_Application_Src})

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${MX_LINK_DIRS})

# Add libraries to the project
target_link_libraries(${CMAKE_PROJECT_NAME} ${MX_LINK_LIBS})

# Add the map file to the list of files to be removed with 'clean' target
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.map)

# Validate that STM32CubeMX code is compatible with C standard
if ((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
    message(ERROR "Generated code requires C11 or higher")
endif ()
