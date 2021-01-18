cmake_minimum_required(VERSION 3.17)
project(HITSIC_CH32V103_MRS)

set(CMAKE_CXX_STANDARD 14)

include_directories(board)
include_directories(component)
include_directories(Core)
include_directories(device)
include_directories(drivers)

add_executable(HITSIC_CH32V103_MRS
        board/board.c
        board/board.h
        component/debug.c
        component/debug.h
        Core/core_riscv.c
        Core/core_riscv.h
        device/ch32v10x.h
        device/ch32v10x_conf.h
        device/system_ch32v10x.c
        device/system_ch32v10x.h
        drivers/ch32v10x_adc.c
        drivers/ch32v10x_adc.h
        drivers/ch32v10x_bkp.c
        drivers/ch32v10x_bkp.h
        drivers/ch32v10x_crc.c
        drivers/ch32v10x_crc.h
        drivers/ch32v10x_dbgmcu.c
        drivers/ch32v10x_dbgmcu.h
        drivers/ch32v10x_dma.c
        drivers/ch32v10x_dma.h
        drivers/ch32v10x_exti.c
        drivers/ch32v10x_exti.h
        drivers/ch32v10x_flash.c
        drivers/ch32v10x_flash.h
        drivers/ch32v10x_gpio.c
        drivers/ch32v10x_gpio.h
        drivers/ch32v10x_i2c.c
        drivers/ch32v10x_i2c.h
        drivers/ch32v10x_iwdg.c
        drivers/ch32v10x_iwdg.h
        drivers/ch32v10x_misc.c
        drivers/ch32v10x_misc.h
        drivers/ch32v10x_pwr.c
        drivers/ch32v10x_pwr.h
        drivers/ch32v10x_rcc.c
        drivers/ch32v10x_rcc.h
        drivers/ch32v10x_rtc.c
        drivers/ch32v10x_rtc.h
        drivers/ch32v10x_spi.c
        drivers/ch32v10x_spi.h
        drivers/ch32v10x_tim.c
        drivers/ch32v10x_tim.h
        drivers/ch32v10x_usart.c
        drivers/ch32v10x_usart.h
        drivers/ch32v10x_usb.c
        drivers/ch32v10x_usb.h
        drivers/ch32v10x_usb_host.c
        drivers/ch32v10x_usb_host.h
        drivers/ch32v10x_wwdg.c
        drivers/ch32v10x_wwdg.h
        source/isr.c
        source/main.cpp
        source/syscalls.c sc_library/sc_common.h)
