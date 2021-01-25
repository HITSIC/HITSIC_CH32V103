//
// Created by jerry on 2021/1/19.
//

#ifndef HITSIC_CH32V103_MRS_SC_UART_H
#define HITSIC_CH32V103_MRS_SC_UART_H

#include "ch32v10x_usart.h"
#include "sc_common.h"
#include "sc_gpio.h"
/*!
 * @addtogroup uart_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief UART driver version 2.3.0. */
#define WCH_UART_DRIVER_VERSION (MAKE_VERSION(1, 0, 0))
/*@}*/

/*! @brief Retry times for waiting flag. */
#ifndef UART_RETRY_TIMES
#define UART_RETRY_TIMES 0U /* Defining to zero means to keep waiting for the flag until it is assert/deassert. */
#endif

/*! @brief Error codes for the UART driver. */
enum
{
    kStatus_UART_TxBusy              = MAKE_STATUS(kStatusGroup_UART, 0), /*!< Transmitter is busy. */
    kStatus_UART_RxBusy              = MAKE_STATUS(kStatusGroup_UART, 1), /*!< Receiver is busy. */
    kStatus_UART_TxIdle              = MAKE_STATUS(kStatusGroup_UART, 2), /*!< UART transmitter is idle. */
    kStatus_UART_RxIdle              = MAKE_STATUS(kStatusGroup_UART, 3), /*!< UART receiver is idle. */
    kStatus_UART_TxWatermarkTooLarge = MAKE_STATUS(kStatusGroup_UART, 4), /*!< TX FIFO watermark too large  */
    kStatus_UART_RxWatermarkTooLarge = MAKE_STATUS(kStatusGroup_UART, 5), /*!< RX FIFO watermark too large  */
    kStatus_UART_FlagCannotClearManually =
    MAKE_STATUS(kStatusGroup_UART, 6),                                 /*!< UART flag can't be manually cleared. */
    kStatus_UART_Error               = MAKE_STATUS(kStatusGroup_UART, 7),  /*!< Error happens on UART. */
    kStatus_UART_RxRingBufferOverrun = MAKE_STATUS(kStatusGroup_UART, 8),  /*!< UART RX software ring buffer overrun. */
    kStatus_UART_RxHardwareOverrun   = MAKE_STATUS(kStatusGroup_UART, 9),  /*!< UART RX receiver overrun. */
    kStatus_UART_NoiseError          = MAKE_STATUS(kStatusGroup_UART, 10), /*!< UART noise error. */
    kStatus_UART_FramingError        = MAKE_STATUS(kStatusGroup_UART, 11), /*!< UART framing error. */
    kStatus_UART_ParityError         = MAKE_STATUS(kStatusGroup_UART, 12), /*!< UART parity error. */
    kStatus_UART_BaudrateNotSupport =
    MAKE_STATUS(kStatusGroup_UART, 13), /*!< Baudrate is not support in current clock source */
    kStatus_UART_IdleLineDetected = MAKE_STATUS(kStatusGroup_UART, 14), /*!< UART IDLE line detected. */
    kStatus_UART_Timeout          = MAKE_STATUS(kStatusGroup_UART, 15), /*!< UART times out. */
};

/*! @brief UART parity mode. */
typedef enum _uart_parity_mode
{
    kUART_ParityDisabled = 0x0U, /*!< Parity disabled */
    kUART_ParityEven     = 0x2U, /*!< Parity enabled, type even, bit setting: PE|PT = 10 */
    kUART_ParityOdd      = 0x3U, /*!< Parity enabled, type odd,  bit setting: PE|PT = 11 */
} uart_parity_mode_t;

/*! @brief UART stop bit count. */
typedef enum _uart_stop_bit_count
{
    kUART_OneStopBit = 0U, /*!< One stop bit */
    kUART_TwoStopBit = 1U, /*!< Two stop bits */
} uart_stop_bit_count_t;

/*! @brief UART idle type select. */
typedef enum _uart_idle_type_select
{
    kUART_IdleTypeStartBit = 0U, /*!< Start counting after a valid start bit. */
    kUART_IdleTypeStopBit  = 1U, /*!< Start counting after a stop bit. */
} uart_idle_type_select_t;

///*!
// * @brief UART interrupt configuration structure, default settings all disabled.
// *
// * This structure contains the settings for all of the UART interrupt configurations.
// */
//enum _uart_interrupt_enable
//{
//#if defined(WCH_FEATURE_UART_HAS_LIN_BREAK_DETECT) && WCH_FEATURE_UART_HAS_LIN_BREAK_DETECT
//    kUART_LinBreakInterruptEnable = (UART_BDH_LBKDIE_MASK), /*!< LIN break detect interrupt. */
//#endif
//    kUART_RxActiveEdgeInterruptEnable         = (UART_BDH_RXEDGIE_MASK), /*!< RX active edge interrupt. */
//    kUART_TxDataRegEmptyInterruptEnable       = (UART_C2_TIE_MASK << 8), /*!< Transmit data register empty interrupt. */
//    kUART_TransmissionCompleteInterruptEnable = (UART_C2_TCIE_MASK << 8), /*!< Transmission complete interrupt. */
//    kUART_RxDataRegFullInterruptEnable        = (UART_C2_RIE_MASK << 8),  /*!< Receiver data register full interrupt. */
//    kUART_IdleLineInterruptEnable             = (UART_C2_ILIE_MASK << 8), /*!< Idle line interrupt. */
//    kUART_RxOverrunInterruptEnable            = (UART_C3_ORIE_MASK << 16), /*!< Receiver overrun interrupt. */
//    kUART_NoiseErrorInterruptEnable           = (UART_C3_NEIE_MASK << 16), /*!< Noise error flag interrupt. */
//    kUART_FramingErrorInterruptEnable         = (UART_C3_FEIE_MASK << 16), /*!< Framing error flag interrupt. */
//    kUART_ParityErrorInterruptEnable          = (UART_C3_PEIE_MASK << 16), /*!< Parity error flag interrupt. */
//#if defined(WCH_FEATURE_UART_HAS_FIFO) && WCH_FEATURE_UART_HAS_FIFO
//    kUART_RxFifoOverflowInterruptEnable  = (UART_CFIFO_RXOFE_MASK << 24), /*!< RX FIFO overflow interrupt. */
//    kUART_TxFifoOverflowInterruptEnable  = (UART_CFIFO_TXOFE_MASK << 24), /*!< TX FIFO overflow interrupt. */
//    kUART_RxFifoUnderflowInterruptEnable = (UART_CFIFO_RXUFE_MASK << 24), /*!< RX FIFO underflow interrupt. */
//#endif
//    kUART_AllInterruptsEnable =
//#if defined(WCH_FEATURE_UART_HAS_LIN_BREAK_DETECT) && WCH_FEATURE_UART_HAS_LIN_BREAK_DETECT
//kUART_LinBreakInterruptEnable |
//#endif
//kUART_RxActiveEdgeInterruptEnable | kUART_TxDataRegEmptyInterruptEnable |
//kUART_TransmissionCompleteInterruptEnable | kUART_RxDataRegFullInterruptEnable | kUART_IdleLineInterruptEnable |
//kUART_RxOverrunInterruptEnable | kUART_NoiseErrorInterruptEnable | kUART_FramingErrorInterruptEnable |
//kUART_ParityErrorInterruptEnable
//#if defined(WCH_FEATURE_UART_HAS_FIFO) && WCH_FEATURE_UART_HAS_FIFO
//    | kUART_RxFifoOverflowInterruptEnable | kUART_TxFifoOverflowInterruptEnable |
//        kUART_RxFifoUnderflowInterruptEnable
//#endif
//    ,
//};

///*!
// * @brief UART status flags.
// *
// * This provides constants for the UART status flags for use in the UART functions.
// */
//enum _uart_flags
//{
//    kUART_TxDataRegEmptyFlag       = (UART_S1_TDRE_MASK), /*!< TX data register empty flag. */
//    kUART_TransmissionCompleteFlag = (UART_S1_TC_MASK),   /*!< Transmission complete flag. */
//    kUART_RxDataRegFullFlag        = (UART_S1_RDRF_MASK), /*!< RX data register full flag. */
//    kUART_IdleLineFlag             = (UART_S1_IDLE_MASK), /*!< Idle line detect flag. */
//    kUART_RxOverrunFlag            = (UART_S1_OR_MASK),   /*!< RX overrun flag. */
//    kUART_NoiseErrorFlag           = (UART_S1_NF_MASK),   /*!< RX takes 3 samples of each received bit.
//                                                               If any of these samples differ, noise flag sets */
//    kUART_FramingErrorFlag = (UART_S1_FE_MASK),           /*!< Frame error flag, sets if logic 0 was detected
//                                                               where stop bit expected */
//    kUART_ParityErrorFlag = (UART_S1_PF_MASK),            /*!< If parity enabled, sets upon parity error detection */
//#if defined(WCH_FEATURE_UART_HAS_LIN_BREAK_DETECT) && WCH_FEATURE_UART_HAS_LIN_BREAK_DETECT
//    kUART_LinBreakFlag =
//        (UART_S2_LBKDIF_MASK
//         << 8), /*!< LIN break detect interrupt flag, sets when LIN break char detected and LIN circuit enabled */
//#endif
//    kUART_RxActiveEdgeFlag =
//    (UART_S2_RXEDGIF_MASK << 8), /*!< RX pin active edge interrupt flag,sets when active edge detected */
//    kUART_RxActiveFlag =
//    (UART_S2_RAF_MASK << 8), /*!< Receiver Active Flag (RAF), sets at beginning of valid start bit */
//#if defined(WCH_FEATURE_UART_HAS_EXTENDED_DATA_REGISTER_FLAGS) && WCH_FEATURE_UART_HAS_EXTENDED_DATA_REGISTER_FLAGS
//    kUART_NoiseErrorInRxDataRegFlag  = (UART_ED_NOISY_MASK << 16),   /*!< Noisy bit, sets if noise detected. */
//    kUART_ParityErrorInRxDataRegFlag = (UART_ED_PARITYE_MASK << 16), /*!< Parity bit, sets if parity error detected. */
//#endif
//#if defined(WCH_FEATURE_UART_HAS_FIFO) && WCH_FEATURE_UART_HAS_FIFO
//    kUART_TxFifoEmptyFlag     = (int)(UART_SFIFO_TXEMPT_MASK << 24), /*!< TXEMPT bit, sets if TX buffer is empty */
//    kUART_RxFifoEmptyFlag     = (UART_SFIFO_RXEMPT_MASK << 24),      /*!< RXEMPT bit, sets if RX buffer is empty */
//    kUART_TxFifoOverflowFlag  = (UART_SFIFO_TXOF_MASK << 24), /*!< TXOF bit, sets if TX buffer overflow occurred */
//    kUART_RxFifoOverflowFlag  = (UART_SFIFO_RXOF_MASK << 24), /*!< RXOF bit, sets if receive buffer overflow */
//    kUART_RxFifoUnderflowFlag = (UART_SFIFO_RXUF_MASK << 24), /*!< RXUF bit, sets if receive buffer underflow */
//#endif
//};

/*! @brief UART transfer structure. */
typedef struct _uart_transfer
{
    uint8_t *data;   /*!< The buffer of data to be transfer.*/
    size_t dataSize; /*!< The byte count to be transfer. */
} uart_transfer_t;

/* Forward declaration of the handle typedef. */
typedef struct _uart_handle uart_handle_t;

///*! @brief UART transfer callback function. */
//typedef void (*uart_transfer_callback_t)(UART_Type *base, uart_handle_t *handle, status_t status, void *userData);

/*! @brief UART handle structure. */
struct _uart_handle
{
    uint8_t *volatile txData;   /*!< Address of remaining data to send. */
    volatile size_t txDataSize; /*!< Size of the remaining data to send. */
    size_t txDataSizeAll;       /*!< Size of the data to send out. */
    uint8_t *volatile rxData;   /*!< Address of remaining data to receive. */
    volatile size_t rxDataSize; /*!< Size of the remaining data to receive. */
    size_t rxDataSizeAll;       /*!< Size of the data to receive. */

    uint8_t *rxRingBuffer;              /*!< Start address of the receiver ring buffer. */
    size_t rxRingBufferSize;            /*!< Size of the ring buffer. */
    volatile uint16_t rxRingBufferHead; /*!< Index for the driver to store received data into ring buffer. */
    volatile uint16_t rxRingBufferTail; /*!< Index for the user to get data from the ring buffer. */

    //uart_transfer_callback_t callback; /*!< Callback function. */
    void *userData;                    /*!< UART callback function parameter.*/

    volatile uint8_t txState; /*!< TX transfer state. */
    volatile uint8_t rxState; /*!< RX transfer state */
};

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

void UART_GetDefaultConfig(USART_InitTypeDef *config);
status_t UART_InitWithPins(USART_TypeDef *base, const USART_InitTypeDef *config, GPIO_Pin tx, GPIO_Pin rx) ;
status_t UART_WriteBlocking(USART_TypeDef* base, const uint8_t* data, size_t length);
static void UART_WriteNonBlocking(USART_TypeDef* base, const uint8_t* data, size_t length);
status_t UART_ReadBlocking(USART_TypeDef* base, uint8_t* data, size_t length);
static void UART_ReadNonBlocking(USART_TypeDef* base, uint8_t* data, size_t length);
	
void UART_PutChar(USART_TypeDef *base, uint8_t dat);

#if defined(__cplusplus)
}
#endif /* _cplusplus */

#endif //HITSIC_CH32V103_MRS_SC_UART_H
