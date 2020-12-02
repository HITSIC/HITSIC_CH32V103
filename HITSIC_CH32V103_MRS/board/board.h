
#ifndef _BOARD_H_
#define _BOARD_H_

#include "ch32v10x.h"

/* UART Printf Definition */
#define DEBUG_UART1    1
#define DEBUG_UART2    2
#define DEBUG_UART3    3

/* DEBUG UATR Definition */
#define DEBUG_UART   DEBUG_UART1


/**
 * @brief   The board name
 */
#define BOARD_NAME "board"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void USART_Printf_Init(uint32_t baudrate);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
