/********************************** (C) COPYRIGHT  *******************************
* File Name          : debug.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/04/30
* Description        : This file contains all the functions prototypes for UART
*                      Printf , Delay functions.
*******************************************************************************/
#include "debug.h"

static uint8_t  p_us=0;
static uint16_t p_ms=0;

/*******************************************************************************
* Function Name  : Delay_Init
* Description    : Initializes Delay Funcation.
* Input          : None
* Return         : None
*******************************************************************************/
void Delay_Init(void)
{
	p_us=SystemCoreClock/8000000;
	p_ms=(uint16_t)p_us*1000;
}

/*******************************************************************************
* Function Name  : Delay_Us
* Description    : Microsecond Delay Time.
* Input          : n£ºMicrosecond number.
* Return         : None
*******************************************************************************/
void Delay_Us(uint32_t n)
{
	uint32_t i;

	SysTick->CTLR = 0;
	i = (uint32_t)n*p_us;

	SysTick->CNTL0 = 0;
	SysTick->CNTL1 = 0;
	SysTick->CNTL2 = 0;
	SysTick->CNTL3 = 0;
	SysTick->CTLR = 1;

  while((*(__IO uint32_t*)0xE000F004) <= i);

}

/*******************************************************************************
* Function Name  : Delay_Ms
* Description    : Millisecond Delay Time.
* Input          : n£ºMillisecond number.
* Return         : None
*******************************************************************************/
void Delay_Ms (uint32_t n)
{
	uint32_t i;

	SysTick->CTLR = 0;
	i = (uint32_t)n*p_ms;

	SysTick->CNTL0 = 0;
	SysTick->CNTL1 = 0;
	SysTick->CNTL2 = 0;
	SysTick->CNTL3 = 0;
	SysTick->CTLR = 1;

  while((*(__IO uint32_t*)0xE000F004) <= i);
}

