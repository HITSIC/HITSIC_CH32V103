//
// Created by jerry on 2021/1/20.
//

#ifndef HITSIC_CH32V103_MRS_UNIT_TEST_H
#define HITSIC_CH32V103_MRS_UNIT_TEST_H

#include "sc_common.h"
#include "sc_gpio.h"


void TEST_GPIO(void)
{
    GPIO_InitTypeDef config;

    GPIO_GetDefaultConfig_Output(&config);

    GPIO_QuickInit(B15,kGPIO_DigitalOutput,1,&config);

    while(1)
    {
        GPIO_Toggle(B15);
        Delay_Ms(500);
    }
}



#endif //HITSIC_CH32V103_MRS_UNIT_TEST_H
