/**
 * Copyright 2018 - 2021 HITSIC
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "board.h"
#include "debug.h"

#include "../HITSIC_Module/d_ch32v103/hitsic_common.h"

#include "vector"

std::vector<int> abc;

#include "sc_unit_test.h"

#define PRINTF_ENABLE 1

extern "C" {
    extern void __libc_init_array(void);
}



/* Global typedef */

/* Global define */

/* Global Variable */


/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Return         : None
*******************************************************************************/
extern "C" int main(void)
{
    /** C++ 底层初始化 */
    /*
     * @note ResetISR似乎没有进行构造操作，我们手动进行。
     */
	__libc_init_array();

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    Delay_Init();
 
	/*USART Init Sequence*/
    USART_InitTypeDef uart_config;
    UART_GetDefaultConfig(&uart_config);
    UART_InitWithPins(USART1, &uart_config, B6, B7);

    /*TODO: PRINTF Redirection not functioning*/
//  setvbuf(stdout,NULL,_IONBF,0);
//	printf("This is printf example\r\n");
//	printf("SystemClk:%ld\r\n", SystemCoreClock);

    GPIO_InitTypeDef config;
    GPIO_GetDefaultConfig_Output(&config);
    GPIO_QuickInit(B15,kGPIO_DigitalOutput,1,&config);


    abc[0] = 3;

	while(1)
    {
	    UART_PutChar(USART1, 0x33);
	    Delay_Ms(500);
        GPIO_Toggle(B15);
    }

    TEST_GPIO();


}

