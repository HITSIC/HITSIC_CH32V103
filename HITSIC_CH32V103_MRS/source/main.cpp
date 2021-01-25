/**
 * Copyright 2018 - 2020 HITSIC
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

#include "vector"

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
    //__libc_init_array();//ResetISR似乎没有进行构造操作，我们手动进行。
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    Delay_Init();
	USART_Printf_Init(115200);

	std::vector<int> vecTest;

	printf("This is printf example\r\n");

	printf("SystemClk:%ld\r\n", SystemCoreClock);

	while(1)
    {
	}
}
