/********************************** (C) COPYRIGHT  *******************************
* File Name          : debug.h
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/04/30
* Description        : This file contains all the functions prototypes for UART
*                      Printf , Delay functions.
*******************************************************************************/
#ifndef __DEBUG_H
#define __DEBUG_H

#include "stdio.h"
#include "ch32v10x.h"

void Delay_Init(void);
void Delay_Us (uint32_t n);
void Delay_Ms (uint32_t n);


#endif /* __DEBUG_H */



