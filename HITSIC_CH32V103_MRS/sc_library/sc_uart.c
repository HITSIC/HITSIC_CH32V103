//
// Created by jerry on 2021/1/15.
//

#include "sc_uart.h"

#include "ch32v10x_crc.h"
#include "ch32v10x_gpio.h"
#include "ch32v10x_usart.h"
#include "ch32v10x_misc.h"

#include "sc_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* UART transfer state. */
enum
{
    kUART_TxIdle,         /* TX idle. */
    kUART_TxBusy,         /* TX busy. */
    kUART_RxIdle,         /* RX idle. */
    kUART_RxBusy,         /* RX busy. */
    kUART_RxFramingError, /* Rx framing error */
    kUART_RxParityError   /* Rx parity error */
};


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
///*!
// * @brief Check whether the RX ring buffer is full.
// *
// * @param handle UART handle pointer.
// * @retval true  RX ring buffer is full.
// * @retval false RX ring buffer is not full.
// */
//static bool UART_TransferIsRxRingBufferFull(uart_handle_t *handle);
//
///*!
// * @brief Read RX register using non-blocking method.
// *
// * This function reads data from the TX register directly, upper layer must make
// * sure the RX register is full or TX FIFO has data before calling this function.
// *
// * @param base UART peripheral base address.
// * @param data Start address of the buffer to store the received data.
// * @param length Size of the buffer.
// */
//static void UART_ReadNonBlocking(USART_TypeDef *base, uint8_t *data, size_t length);
//
///*!
// * @brief Write to TX register using non-blocking method.
// *
// * This function writes data to the TX register directly, upper layer must make
// * sure the TX register is empty or TX FIFO has empty room before calling this function.
// *
// * @note This function does not check whether all the data has been sent out to bus,
// * so before disable TX, check kUART_TransmissionCompleteFlag to ensure the TX is
// * finished.
// *
// * @param base UART peripheral base address.
// * @param data Start address of the data to write.
// * @param length Size of the buffer to be sent.
// */
//static void UART_WriteNonBlocking(USART_TypeDef *base, const uint8_t *data, size_t length);

/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

/*!
 * brief Gets the default configuration structure.
 *
 * This function initializes the UART configuration structure to a default value. The default
 * values are as follows.
 *
 * @code
    SART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
 * @endcode
 *
 * param config Pointer to configuration structure.
 */
void UART_GetDefaultConfig(USART_InitTypeDef *config)
{
    assert(config);

    /* Initializes the configure structure to zero. */
    (void)memset(config, 0, sizeof(*config));

    config->USART_BaudRate = 115200;
    config->USART_WordLength = USART_WordLength_8b;
    config->USART_StopBits = USART_StopBits_1;
    config->USART_Parity = USART_Parity_No;
    config->USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    config->USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

}

status_t UART_InitWithPins(USART_TypeDef *base, const USART_InitTypeDef *config, GPIO_Pin tx, GPIO_Pin rx) {

    assert(config);

    //Enable APB Main Clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* Init USART pins (and possible pin remap), then enable APB USART*/
    GPIO_InitTypeDef  tx_config, rx_config;

    tx_config.GPIO_Speed = GPIO_Speed_50MHz;
    tx_config.GPIO_Mode = GPIO_Mode_AF_PP;

    rx_config.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    if(USART1_BASE ==  base){
        if(A9 == tx && A10 == rx)
        {
            GPIO_QuickInit(A9, kGPIO_DigitalOutput, 1, &tx_config);
            GPIO_QuickInit(A10, kGPIO_DigitalInput, 1 ,&rx_config);
        }
        else if (B6 == tx && B7 == rx)
        {
            GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

            GPIO_QuickInit(B6, kGPIO_DigitalOutput, 1, &tx_config);
            GPIO_QuickInit(B7, kGPIO_DigitalInput, 1 ,&rx_config);
        }
        else
        {
            return kStatus_Fail;
        }

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    }
    else if(USART2_BASE ==  base){
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);


    }
    else if(USART3_BASE ==  base)    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);


    }
    else{
        return kStatus_Fail;
    }

    //Enable USART
    USART_Init((USART_TypeDef*)base, config);
    USART_Cmd((USART_TypeDef*)base, ENABLE);

    return kStatus_Success;
}


#if defined(__cplusplus)
}
#endif /* _cplusplus */

void UART_PutChar(USART_TypeDef *base, uint8_t dat)
{
    while(((base)->STATR & USART_FLAG_TXE)==0);
    (base)->DATAR = dat;
}
