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
	/* TX idle. */
	kUART_TxIdle,

	/* TX busy. */
	kUART_TxBusy,

	/* RX idle. */
	kUART_RxIdle,

	/* RX busy. */
	kUART_RxBusy,

	/* Rx framing error */
	kUART_RxFramingError,

	/* Rx parity error */
	kUART_RxParityError
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

/*!
 * @brief Read RX register using non-blocking method.
 *
 * This function reads data from the TX register directly, upper layer must make
 * sure the RX register is full or TX FIFO has data before calling this function.
 *
 * @param base UART peripheral base address.
 * @param data Start address of the buffer to store the received data.
 * @param length Size of the buffer.
 */
static void UART_ReadNonBlocking(USART_TypeDef* base, uint8_t* data, size_t length);

/*!
 * @brief Write to TX register using non-blocking method.
 *
 * This function writes data to the TX register directly, upper layer must make
 * sure the TX register is empty or TX FIFO has empty room before calling this function.
 *
 * @note This function does not check whether all the data has been sent out to bus,
 * so before disable TX, check kUART_TransmissionCompleteFlag to ensure the TX is
 * finished.
 *
 * @param base UART peripheral base address.
 * @param data Start address of the data to write.
 * @param length Size of the buffer to be sent.
 */
static void UART_WriteNonBlocking(USART_TypeDef* base, const uint8_t* data, size_t length);

/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

/*!
 * @brief Gets the default configuration structure.
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
 * @param config Pointer to configuration structure.
 */
void UART_GetDefaultConfig(USART_InitTypeDef* config)
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


/*!
 * @brief	Init UART with selected pins.
 * @param base USART1...USART3
 * @param config configuration structure
 * @param tx tx_pins
 * @param rx rx_pins
 * @return kStatusSuccess when success	
 *	kStatusFail when choose unpaired TX-RX
*/
status_t UART_InitWithPins(USART_TypeDef* base, const USART_InitTypeDef* config, GPIO_Pin tx, GPIO_Pin rx)
{
	assert(config);

	//Enable APB Main Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/* Init USART pins (and possible pin remap), then enable APB USART*/
	GPIO_InitTypeDef tx_config, rx_config;

	tx_config.GPIO_Speed = GPIO_Speed_50MHz;
	tx_config.GPIO_Mode = GPIO_Mode_AF_PP;

	rx_config.GPIO_Mode = GPIO_Mode_IN_FLOATING;

	/**
	    @attention  when use remap pins as USART output, you MUST choose GPIO_Mode_AF_PP instead of GPIO_Mode_Out_PP as its modes.
	**/
	if (USART1_BASE == base)
	{
		if (A9 == tx && A10 == rx)
		{
			GPIO_QuickInit(A9, kGPIO_DigitalOutput, 1, &tx_config);
			GPIO_QuickInit(A10, kGPIO_DigitalInput, 1, &rx_config);
		}
		else if (B6 == tx && B7 == rx)
		{
			GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

			GPIO_QuickInit(B6, kGPIO_DigitalOutput, 1, &tx_config);
			GPIO_QuickInit(B7, kGPIO_DigitalInput, 1, &rx_config);
		}
		else
		{
			return kStatus_Fail;
		}

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	}
	else if (USART2_BASE == base)
	{
		if (A2 == tx && A3 == rx)
		{
			GPIO_QuickInit(A2, kGPIO_DigitalOutput, 1, &tx_config);
			GPIO_QuickInit(A3, kGPIO_DigitalInput, 1, &rx_config);
		}
		else if (B6 == tx && B7 == rx)
		{
			GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

			GPIO_QuickInit(B10, kGPIO_DigitalOutput, 1, &tx_config);
			GPIO_QuickInit(B11, kGPIO_DigitalInput, 1, &rx_config);
		}
		else
		{
			return kStatus_Fail;
		}
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	}
	else if (USART3_BASE == base)
	{
		if (A2 == tx && A3 == rx)
		{
			GPIO_QuickInit(B10, kGPIO_DigitalOutput, 1, &tx_config);
			GPIO_QuickInit(B11, kGPIO_DigitalInput, 1, &rx_config);
		}
		else if (B6 == tx && B7 == rx)
		{
			GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

			GPIO_QuickInit(C10, kGPIO_DigitalOutput, 1, &tx_config);
			GPIO_QuickInit(C11, kGPIO_DigitalInput, 1, &rx_config);
		}
		else
		{
			return kStatus_Fail;
		}
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	}
	else
	{
		return kStatus_Fail;
	}

	//Enable USART
	USART_Init((USART_TypeDef*)base, config);
	USART_Cmd((USART_TypeDef*)base, ENABLE);

	return kStatus_Success;
}

/*!
 * brief Writes to the TX register using a blocking method.
 *
 * This function polls the TX register, waits for the TX register to be empty or for the TX FIFO
 * to have room and writes data to the TX buffer.
 *
 * param base USART peripheral base address.
 * param data Start address of the data to write.
 * param length Size of the data to write.
 * retval kStatus_UART_Timeout Transmission timed out and was aborted.
 * retval kStatus_Success Successfully wrote all data.
 */
status_t UART_WriteBlocking(USART_TypeDef* base, const uint8_t* data, size_t length)
{
#if UART_RETRY_TIMES
	uint32_t waitTimes;
#endif
	while (0U != length--)
	{
#if UART_RETRY_TIMES
		waitTimes = UART_RETRY_TIMES;
		while ((0U == (base->STATR & USART_FLAG_TXE)) && (0U != --waitTimes))
#else
		while (0U == (base->STATR & USART_FLAG_TXE))
#endif
		{
		}
#if UART_RETRY_TIMES
		if (waitTimes == 0U)
		{
			return kStatus_UART_Timeout;
		}
#endif
		base->DATAR = *(data++);
	}

	/* Ensure all the data in the transmit buffer are sent out to bus. */
#if UART_RETRY_TIMES
	waitTimes = UART_RETRY_TIMES;
	while ((0U == (base->STATR & USART_FLAG_TXE)) && (0U != --waitTimes))
#else
	while (0U == (base->STATR & USART_FLAG_TXE))
#endif
	{
	}
#if UART_RETRY_TIMES
	if (waitTimes == 0U)
	{
		return kStatus_UART_Timeout;
	}
#endif
	return kStatus_Success;
}
	
static void UART_WriteNonBlocking(USART_TypeDef* base, const uint8_t* data, size_t length)
{
	assert(data);

	size_t i;

	/* The Non Blocking write data API assume user have ensured there is enough space in
	peripheral to write. */
	for (i = 0; i < length; i++)
	{
		(base)->DATAR = data[i];
	}
}


/*!
 * @brief Read RX data register using a blocking method.
 *
 * This function polls the RX register, waits for the RX register to be full or for RX FIFO to
 * have data, and reads data from the TX register.
 *
 * @param base UART peripheral base address.
 * @param data Start address of the buffer to store the received data.
 * @param length Size of the buffer.
 * 
 * @retval kStatus_UART_RxHardwareOverrun Receiver overrun occurred while receiving data.
 * @retval kStatus_UART_NoiseError A noise error occurred while receiving data.
 * @retval kStatus_UART_FramingError A framing error occurred while receiving data.
 * @retval kStatus_UART_ParityError A parity error occurred while receiving data.
 * @retval kStatus_UART_Timeout Transmission timed out and was aborted.
 * @retval kStatus_Success Successfully received all data.
 */
status_t UART_ReadBlocking(USART_TypeDef* base, uint8_t* data, size_t length)
{
	assert(data != NULL);

	status_t status = kStatus_Success;
	uint32_t statusFlag;
#if UART_RETRY_TIMES
	uint32_t waitTimes;
#endif

	while (length-- != 0U)
	{
#if UART_RETRY_TIMES
		waitTimes = UART_RETRY_TIMES;
#endif
#if defined(WCH_FEATURE_UART_HAS_FIFO) && WCH_FEATURE_UART_HAS_FIFO
		while (base->RCFIFO == 0U)
#else
		while ((base->STATR & USART_FLAG_RXNE) == 0U)
#endif
		{
#if UART_RETRY_TIMES
			if (0U == --waitTimes)
			{
				status = kStatus_UART_Timeout;
				break;
			}
#endif
			if (0U != (base->STATR & (uint32_t)USART_FLAG_ORE))
			{
				USART_ClearFlag(base, (uint32_t)USART_FLAG_ORE);
				status = (0U != (base->STATR & (uint32_t)USART_FLAG_ORE)) ?
					(status_t)(kStatus_UART_RxHardwareOverrun) :
					(status_t)(kStatus_UART_FlagCannotClearManually);
				/* If the OR bit is set all the other error flags are prevented from setting,
				   no need to check other status flags. */
				break;
			}

			if (0U != (base->STATR & (uint32_t)USART_FLAG_PE))
			{
				USART_ClearFlag(base, (uint32_t)USART_FLAG_PE);
				status = (0U != (base->STATR & (uint32_t)USART_FLAG_PE)) ?
					(status_t)(kStatus_UART_ParityError) :
					(status_t)(kStatus_UART_FlagCannotClearManually);
			}

			if (0U != (base->STATR & (uint32_t)USART_FLAG_FE))
			{
				USART_ClearFlag(base, (uint32_t)USART_FLAG_FE);
				status = (0U != (base->STATR & (uint32_t)USART_FLAG_FE)) ?
					(status_t)(kStatus_UART_FramingError) :
					(status_t)(kStatus_UART_FlagCannotClearManually);
			}

			if (0U != (base->STATR & (uint32_t)USART_FLAG_NE))
			{
				USART_ClearFlag(base, (uint32_t)USART_FLAG_NE);
				status = (0U != (base->STATR & (uint32_t)USART_FLAG_NE)) ?
					(status_t)(kStatus_UART_NoiseError) :
					(status_t)(kStatus_UART_FlagCannotClearManually);
			}
			if (kStatus_Success != status)
			{
				break;
			}
		}
		if (kStatus_Success == status)
		{
			*(data++) = base->DATAR;
		}
		else
		{
			break;
		}
	}

	return status;
}

static void UART_ReadNonBlocking(USART_TypeDef* base, uint8_t* data, size_t length)
{
	assert(data);

	size_t i;

	/* The Non Blocking read data API assume user have ensured there is enough space in
	peripheral to write. */
	for (i = 0; i < length; i++)
	{
		data[i] = base->DATAR;
	}
}
	
/*!
 * @brief Easy method: UART_PutChar
 * @param base USART peripheral base address.
 * @param data char you want to write.
*/
void UART_PutChar(USART_TypeDef* base, uint8_t data)
{
	while (((base)->STATR & USART_FLAG_TXE) == 0);
	(base)->DATAR = data;
}

	
#if defined(__cplusplus)
}
#endif /* _cplusplus */

