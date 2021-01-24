//
// Created by jerry on 2021/1/15.
//
#include "sc_common.h"
#include "sc_gpio.h"

#include "ch32v10x_rcc.h"
#include "ch32v10x_gpio.h"
#include "ch32v10x_exti.h"

/*!
 * brief Gets the default configuration structure: Output
 *
 * This function initializes the GPIO configuration structure to a default value. The default
 * values are as follows.
 *
 * @code
    config->GPIO_Pin = GPIO_Pin_All;
    config->GPIO_Speed = GPIO_Speed_50MHz;
    config->GPIO_Mode = GPIO_Mode_Out_PP;
 * @endcode
 *
 * @note Please choose the proper mode after get config
 * @code
 *      GPIO_Mode_AIN = 0x0,                //模拟输入模式
        GPIO_Mode_IN_FLOATING = 0x04,       //浮空输入模式
        GPIO_Mode_IPD = 0x28,               //下拉输入
        GPIO_Mode_IPU = 0x48,               //上拉输入
        GPIO_Mode_Out_OD = 0x14,            //通用开漏输出模式
        GPIO_Mode_Out_PP = 0x10,            //通用推挽输出模式
        GPIO_Mode_AF_OD = 0x1C,             //复用功能开漏输出模式
        GPIO_Mode_AF_PP = 0x18              //复用功能推挽输出模式
 *
 * param config Pointer to configuration structure.
 */
void GPIO_GetDefaultConfig_Output(GPIO_InitTypeDef *config)
{
    //config->GPIO_Pin = GPIO_Pin_All;    /*Usually we do not use this member to configure GPIO*/
    config->GPIO_Speed = GPIO_Speed_50MHz;
    config->GPIO_Mode = GPIO_Mode_Out_PP;
}

/*!
 * brief Gets the default configuration structure: Input
 *
 * This function initializes the GPIO configuration structure to a default value. The default
 * values are as follows.
 *
 * @code
    config->GPIO_Mode = GPIO_Mode_IN_FLOATING;
 * @endcode
 *
 * @note Please choose the proper mode after get config
 * @code
 *      GPIO_Mode_AIN = 0x0,                //模拟输入模式
        GPIO_Mode_IN_FLOATING = 0x04,       //浮空输入模式
        GPIO_Mode_IPD = 0x28,               //下拉输入
        GPIO_Mode_IPU = 0x48,               //上拉输入
        GPIO_Mode_Out_OD = 0x14,            //通用开漏输出模式
        GPIO_Mode_Out_PP = 0x10,            //通用推挽输出模式
        GPIO_Mode_AF_OD = 0x1C,             //复用功能开漏输出模式
        GPIO_Mode_AF_PP = 0x18              //复用功能推挽输出模式
  @endcode
 *
 * param config Pointer to configuration structure.
 */
void GPIO_GetDefaultConfig_Input(GPIO_InitTypeDef *config)
{
    //config->GPIO_Pin = GPIO_Pin_All;    /*Usually we do not use this member to configure GPIO*/
    //config->GPIO_Speed = GPIO_Speed_50MHz;
    config->GPIO_Mode = GPIO_Mode_IN_FLOATING;
}


status_t GPIO_QuickInit(GPIO_Pin pin, gpio_pin_direction_t direction, bool init_value, GPIO_InitTypeDef *config)
{
    uint8_t io_group = (pin & 0xE0) >> 5;
    uint8_t io_pin = pin & 0x1F;

    if(PIN_NULL == pin)
        return kStatus_Fail;
    else
        config->GPIO_Pin = (uint16_t)(1 << io_pin);

    /*GPIO APB Init*/
    if(0 == (pin >> 5))
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if(1 == (pin >> 5))
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if(2 == (pin >> 5))
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else if(3 == (pin >> 5))
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    else if(4 == (pin >> 5))
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    else if(5 == (pin >> 5))
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    else if(6 == (pin >> 5))
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
    else kStatus_Fail;

    /*gpio_config: Init Pin with WCH library*/
    GPIO_Init((GPIO_TypeDef *)(GPION[io_group]), config);

    if(kGPIO_DigitalOutput == direction)
    {
        GPIO_WriteBit((GPIO_TypeDef *)(GPION[io_group]), (uint16_t)(1 << io_pin), (uint8_t)init_value);
    }
    return kStatus_Success;
}


void GPIO_Set(GPIO_Pin pin, uint8_t dat)
{
    uint8_t io_group = (pin & 0xE0) >> 5;
    uint8_t io_pin = pin & 0x1F;

    if (dat == 1)
        ((GPIO_TypeDef *)(GPION[io_group]))->BSHR = 1 << io_pin;
    else
        ((GPIO_TypeDef *)(GPION[io_group]))->BCR = 1 << io_pin;
}


uint8_t GPIO_Get(GPIO_Pin pin)
{
    uint8_t io_group = (pin & 0xE0) >> 5;
    uint8_t io_pin = pin & 0x1F;

    if((((GPIO_TypeDef *)(GPION[io_group]))->INDR & (1 << io_pin)) != 0)
    {
        return 1;
    }
    return 0;

}


void GPIO_Toggle(GPIO_Pin pin)
{
    uint8_t io_group = (pin & 0xE0) >> 5;
    uint8_t io_pin = pin & 0x1F;

    ((GPIO_TypeDef *)(GPION[io_group]))->OUTDR ^= (uint16_t)(1 << io_pin);
}
