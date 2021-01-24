//
// Created by jerry on 2021/1/15.
//

#ifndef HITSIC_CH32V103_MRS_SC_GPIO_H
#define HITSIC_CH32V103_MRS_SC_GPIO_H

#include "ch32v10x_gpio.h"
typedef enum
{
    A0 = 0,  A1,  A2,  A3,  A4,  A5,  A6,  A7,
    A8,  A9,  A10, A11, A12, A13, A14, A15,
    A16, A17, A18, A19, A20, A21, A22, A23, //Reserved.
    A24, A25, A26, A27, A28, A29, A30, A31, //Reserved.

    B0,  B1,  B2,  B3,  B4,  B5,  B6,  B7,
    B8,  B9,  B10, B11, B12, B13, B14, B15,
    B16, B17, B18, B19, B20, B21, B22, B23, //Reserved.
    B24, B25, B26, B27, B28, B29, B30, B31, //Reserved.

    C0,  C1,  C2,  C3,  C4,  C5,  C6,  C7,
    C8,  C9,  C10, C11, C12, C13, C14, C15,
    C16, C17, C18, C19, C20, C21, C22, C23, //Reserved.
    C24, C25, C26, C27, C28, C29, C30, C31, //Reserved.

    D0,  D1,  D2,  D3,  D4,  D5,  D6,  D7,
    D8,  D9,  D10, D11, D12, D13, D14, D15,
    D16, D17, D18, D19, D20, D21, D22, D23, //Reserved.
    D24, D25, D26, D27, D28, D29, D30, D31, //Reserved.

    PIN_NULL,
}GPIO_Pin;

const uint32_t GPION[4] = {GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOD_BASE};

typedef enum _gpio_pin_direction
{
    kGPIO_DigitalInput  = 0U, /*!< Set current pin as digital input*/
    kGPIO_DigitalOutput = 1U, /*!< Set current pin as digital output*/
} gpio_pin_direction_t;

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

void GPIO_GetDefaultConfig_Output(GPIO_InitTypeDef *config);
void GPIO_GetDefaultConfig_Input(GPIO_InitTypeDef *config);

status_t GPIO_QuickInit(GPIO_Pin pin, gpio_pin_direction_t direction, bool init_value, GPIO_InitTypeDef *config);

void GPIO_Set(GPIO_Pin pin, uint8_t dat);
uint8_t GPIO_Get(GPIO_Pin pin);
void GPIO_Toggle(GPIO_Pin pin);

#if defined(__cplusplus)
}
#endif

#endif //HITSIC_CH32V103_MRS_SC_GPIO_H
