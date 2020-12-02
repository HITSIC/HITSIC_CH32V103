#include "board.h"
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */



/*******************************************************************************
* Function Name  : USART_Printf_Init
* Description    : Initializes the USARTx peripheral.
* Input          : baudrate: USART communication baud rate.
* Return         : None
*******************************************************************************/
void USART_Printf_Init(uint32_t baudrate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

#if (DEBUG_UART == DEBUG_UART1)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

#elif (DEBUG_UART == DEBUG_UART2)
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

#elif (DEBUG_UART == DEBUG_UART3)
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

#endif

  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx;

#if (DEBUG == DEBUG_UART1)
  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);

#elif (DEBUG_UART == DEBUG_UART2)
  USART_Init(USART2, &USART_InitStructure);
  USART_Cmd(USART2, ENABLE);

#elif (DEBUG_UART == DEBUG_UART3)
  USART_Init(USART3, &USART_InitStructure);
  USART_Cmd(USART3, ENABLE);

#endif
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */
