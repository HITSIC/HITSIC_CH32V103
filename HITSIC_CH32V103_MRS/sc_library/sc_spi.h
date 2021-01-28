//
// Created by jerry on 2021/1/15.
//

#ifndef HITSIC_CH32V103_MRS_SC_SPI_H
#define HITSIC_CH32V103_MRS_SC_SPI_H

#include "ch32v10x_spi.h"
#include "sc_common.h"
#include "sc_gpio.h"

/*Marco for manage unfinished porting code */
#define HITSIC_PORTING_SPI 0U

/*!
 * @addtogroup spi_driver
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

/**********************************************************************************************************************
 * Definitions
 *********************************************************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief SPI driver version 2.2.4. */
#define WCH_SPI_DRIVER_VERSION (MAKE_VERSION(1, 0, 0))
/*@}*/

#ifndef SPI_DUMMY_DATA
/*! @brief DSPI dummy data if there is no Tx data.*/
#define SPI_DUMMY_DATA (0x00U) /*!< Dummy data used for Tx if there is no txData. */
#endif

/*! @brief Global variable for dummy data value setting. */
extern volatile uint8_t g_spiDummyData[];

/*! @brief Status for the DSPI driver.*/
enum
{
	kStatus_SPI_Busy = MAKE_STATUS(kStatusGroup_DSPI, 0),
	/*!< DSPI transfer is busy.*/
	kStatus_SPI_Error = MAKE_STATUS(kStatusGroup_DSPI, 1),
	/*!< DSPI driver error. */
	kStatus_SPI_Idle = MAKE_STATUS(kStatusGroup_DSPI, 2),
	/*!< DSPI is idle.*/
	kStatus_SPI_OutOfRange = MAKE_STATUS(kStatusGroup_DSPI, 3) /*!< DSPI transfer out of range. */
};

#if HITSIC_PORTING_SPI

/*! @brief DSPI status flags in SPIx_SR register.*/
enum _dspi_flags
{
    kDSPI_TxCompleteFlag = (int)SPI_SR_TCF_MASK, /*!< Transfer Complete Flag. */
    kDSPI_EndOfQueueFlag = SPI_SR_EOQF_MASK,     /*!< End of Queue Flag.*/
    kDSPI_TxFifoUnderflowFlag = SPI_SR_TFUF_MASK,     /*!< Transmit FIFO Underflow Flag.*/
    kDSPI_TxFifoFillRequestFlag = SPI_SR_TFFF_MASK,     /*!< Transmit FIFO Fill Flag.*/
    kDSPI_RxFifoOverflowFlag = SPI_SR_RFOF_MASK,     /*!< Receive FIFO Overflow Flag.*/
    kDSPI_RxFifoDrainRequestFlag = SPI_SR_RFDF_MASK,     /*!< Receive FIFO Drain Flag.*/
    kDSPI_TxAndRxStatusFlag = SPI_SR_TXRXS_MASK,    /*!< The module is in Stopped/Running state.*/
    kDSPI_AllStatusFlag = (int)(SPI_SR_TCF_MASK | SPI_SR_EOQF_MASK | SPI_SR_TFUF_MASK | SPI_SR_TFFF_MASK |
    SPI_SR_RFOF_MASK | SPI_SR_RFDF_MASK | SPI_SR_TXRXS_MASK) /*!< All statuses above.*/
};

/*! @brief DSPI interrupt source.*/
enum _dspi_interrupt_enable
{
    kDSPI_TxCompleteInterruptEnable = (int)SPI_RSER_TCF_RE_MASK, /*!< TCF  interrupt enable.*/
    kDSPI_EndOfQueueInterruptEnable = SPI_RSER_EOQF_RE_MASK,     /*!< EOQF interrupt enable.*/
    kDSPI_TxFifoUnderflowInterruptEnable = SPI_RSER_TFUF_RE_MASK,     /*!< TFUF interrupt enable.*/
    kDSPI_TxFifoFillRequestInterruptEnable = SPI_RSER_TFFF_RE_MASK,     /*!< TFFF interrupt enable, DMA disable.*/
    kDSPI_RxFifoOverflowInterruptEnable = SPI_RSER_RFOF_RE_MASK,     /*!< RFOF interrupt enable.*/
    kDSPI_RxFifoDrainRequestInterruptEnable = SPI_RSER_RFDF_RE_MASK,     /*!< RFDF interrupt enable, DMA disable.*/
    kDSPI_AllInterruptEnable = (int)(SPI_RSER_TCF_RE_MASK | SPI_RSER_EOQF_RE_MASK | SPI_RSER_TFUF_RE_MASK |
    SPI_RSER_TFFF_RE_MASK | SPI_RSER_RFOF_RE_MASK | SPI_RSER_RFDF_RE_MASK)
    /*!< All above interrupts enable.*/
};

/*! @brief DSPI DMA source.*/
enum _dspi_dma_enable
{
    kDSPI_TxDmaEnable = (SPI_RSER_TFFF_RE_MASK | SPI_RSER_TFFF_DIRS_MASK), /*!< TFFF flag generates DMA requests.
                                                                                No Tx interrupt request. */
                                                                                kDSPI_RxDmaEnable = (SPI_RSER_RFDF_RE_MASK | SPI_RSER_RFDF_DIRS_MASK)  /*!< RFDF flag generates DMA requests.
                                                                                                                                                            No Rx interrupt request. */
};

/*! @brief DSPI master or slave mode configuration.*/
typedef enum _dspi_master_slave_mode
{
    kDSPI_Master = 1U, /*!< DSPI peripheral operates in master mode.*/
    kDSPI_Slave = 0U  /*!< DSPI peripheral operates in slave mode.*/
} dspi_master_slave_mode_t;

/*!
 * @brief DSPI Sample Point: Controls when the DSPI master samples SIN in the Modified Transfer Format. This field is
 * valid only when the CPHA bit in the CTAR register is 0.
 */
typedef enum _dspi_master_sample_point
{
    kDSPI_SckToSin0Clock = 0U, /*!< 0 system clocks between SCK edge and SIN sample.*/
    kDSPI_SckToSin1Clock = 1U, /*!< 1 system clock  between SCK edge and SIN sample.*/
    kDSPI_SckToSin2Clock = 2U  /*!< 2 system clocks between SCK edge and SIN sample.*/
} dspi_master_sample_point_t;

/*! @brief DSPI Peripheral Chip Select (Pcs) configuration (which Pcs to configure).*/
typedef enum _dspi_which_pcs_config
{
    kDSPI_Pcs0 = 1U << 0, /*!< Pcs[0] */
    kDSPI_Pcs1 = 1U << 1, /*!< Pcs[1] */
    kDSPI_Pcs2 = 1U << 2, /*!< Pcs[2] */
    kDSPI_Pcs3 = 1U << 3, /*!< Pcs[3] */
    kDSPI_Pcs4 = 1U << 4, /*!< Pcs[4] */
    kDSPI_Pcs5 = 1U << 5  /*!< Pcs[5] */
} dspi_which_pcs_t;

/*! @brief DSPI Peripheral Chip Select (Pcs) Polarity configuration.*/
typedef enum _dspi_pcs_polarity_config
{
    kDSPI_PcsActiveHigh = 0U, /*!< Pcs Active High (idles low). */
    kDSPI_PcsActiveLow = 1U  /*!< Pcs Active Low (idles high). */
} dspi_pcs_polarity_config_t;

/*! @brief DSPI Peripheral Chip Select (Pcs) Polarity.*/
enum _dspi_pcs_polarity
{
    kDSPI_Pcs0ActiveLow = 1U << 0, /*!< Pcs0 Active Low (idles high). */
    kDSPI_Pcs1ActiveLow = 1U << 1, /*!< Pcs1 Active Low (idles high). */
    kDSPI_Pcs2ActiveLow = 1U << 2, /*!< Pcs2 Active Low (idles high). */
    kDSPI_Pcs3ActiveLow = 1U << 3, /*!< Pcs3 Active Low (idles high). */
    kDSPI_Pcs4ActiveLow = 1U << 4, /*!< Pcs4 Active Low (idles high). */
    kDSPI_Pcs5ActiveLow = 1U << 5, /*!< Pcs5 Active Low (idles high). */
    kDSPI_PcsAllActiveLow = 0xFFU    /*!< Pcs0 to Pcs5 Active Low (idles high). */
};

/*! @brief DSPI clock polarity configuration for a given CTAR.*/
typedef enum _dspi_clock_polarity
{
    kDSPI_ClockPolarityActiveHigh = 0U, /*!< CPOL=0. Active-high DSPI clock (idles low).*/
    kDSPI_ClockPolarityActiveLow = 1U  /*!< CPOL=1. Active-low DSPI clock (idles high).*/
} dspi_clock_polarity_t;

/*! @brief DSPI clock phase configuration for a given CTAR.*/
typedef enum _dspi_clock_phase
{
    kDSPI_ClockPhaseFirstEdge = 0U, /*!< CPHA=0. Data is captured on the leading edge of the SCK and changed on the
                                         following edge.*/
                                         kDSPI_ClockPhaseSecondEdge = 1U /*!< CPHA=1. Data is changed on the leading edge of the SCK and captured on the
                                                                             following edge.*/
} dspi_clock_phase_t;

/*! @brief DSPI data shifter direction options for a given CTAR.*/
typedef enum _dspi_shift_direction
{
    kDSPI_MsbFirst = 0U, /*!< Data transfers start with most significant bit.*/
    kDSPI_LsbFirst = 1U  /*!< Data transfers start with least significant bit.
                              Shifting out of LSB is not supported for slave */
} dspi_shift_direction_t;

/*! @brief DSPI delay type selection.*/
typedef enum _dspi_delay_type
{
    kDSPI_PcsToSck = 1U,  /*!< Pcs-to-SCK delay. */
    kDSPI_LastSckToPcs,   /*!< The last SCK edge to Pcs delay. */
    kDSPI_BetweenTransfer /*!< Delay between transfers. */
} dspi_delay_type_t;

/*! @brief DSPI Clock and Transfer Attributes Register (CTAR) selection.*/
typedef enum _dspi_ctar_selection
{
    kDSPI_Ctar0 = 0U, /*!< CTAR0 selection option for master or slave mode; note that CTAR0 and CTAR0_SLAVE are the
                         same register address. */
                         kDSPI_Ctar1 = 1U, /*!< CTAR1 selection option for master mode only. */
                         kDSPI_Ctar2 = 2U, /*!< CTAR2 selection option for master mode only; note that some devices do not support CTAR2. */
                         kDSPI_Ctar3 = 3U, /*!< CTAR3 selection option for master mode only; note that some devices do not support CTAR3. */
                         kDSPI_Ctar4 = 4U, /*!< CTAR4 selection option for master mode only; note that some devices do not support CTAR4. */
                         kDSPI_Ctar5 = 5U, /*!< CTAR5 selection option for master mode only; note that some devices do not support CTAR5. */
                         kDSPI_Ctar6 = 6U, /*!< CTAR6 selection option for master mode only; note that some devices do not support CTAR6. */
                         kDSPI_Ctar7 = 7U  /*!< CTAR7 selection option for master mode only; note that some devices do not support CTAR7. */
} dspi_ctar_selection_t;

#define DSPI_MASTER_CTAR_SHIFT (0U)    /*!< DSPI master CTAR shift macro; used internally. */
#define DSPI_MASTER_CTAR_MASK  (0x0FU) /*!< DSPI master CTAR mask macro; used internally. */
#define DSPI_MASTER_PCS_SHIFT  (4U)    /*!< DSPI master PCS shift macro; used internally. */
#define DSPI_MASTER_PCS_MASK   (0xF0U) /*!< DSPI master PCS mask macro; used internally. */
/*! @brief Use this enumeration for the DSPI master transfer configFlags. */
enum _dspi_transfer_config_flag_for_master
{
    kDSPI_MasterCtar0 = 0U << DSPI_MASTER_CTAR_SHIFT, /*!< DSPI master transfer use CTAR0 setting. */
    kDSPI_MasterCtar1 = 1U << DSPI_MASTER_CTAR_SHIFT, /*!< DSPI master transfer use CTAR1 setting. */
    kDSPI_MasterCtar2 = 2U << DSPI_MASTER_CTAR_SHIFT, /*!< DSPI master transfer use CTAR2 setting. */
    kDSPI_MasterCtar3 = 3U << DSPI_MASTER_CTAR_SHIFT, /*!< DSPI master transfer use CTAR3 setting. */
    kDSPI_MasterCtar4 = 4U << DSPI_MASTER_CTAR_SHIFT, /*!< DSPI master transfer use CTAR4 setting. */
    kDSPI_MasterCtar5 = 5U << DSPI_MASTER_CTAR_SHIFT, /*!< DSPI master transfer use CTAR5 setting. */
    kDSPI_MasterCtar6 = 6U << DSPI_MASTER_CTAR_SHIFT, /*!< DSPI master transfer use CTAR6 setting. */
    kDSPI_MasterCtar7 = 7U << DSPI_MASTER_CTAR_SHIFT, /*!< DSPI master transfer use CTAR7 setting. */

    kDSPI_MasterPcs0 = 0U << DSPI_MASTER_PCS_SHIFT, /*!< DSPI master transfer use PCS0 signal. */
    kDSPI_MasterPcs1 = 1U << DSPI_MASTER_PCS_SHIFT, /*!< DSPI master transfer use PCS1 signal. */
    kDSPI_MasterPcs2 = 2U << DSPI_MASTER_PCS_SHIFT, /*!< DSPI master transfer use PCS2 signal.*/
    kDSPI_MasterPcs3 = 3U << DSPI_MASTER_PCS_SHIFT, /*!< DSPI master transfer use PCS3 signal. */
    kDSPI_MasterPcs4 = 4U << DSPI_MASTER_PCS_SHIFT, /*!< DSPI master transfer use PCS4 signal. */
    kDSPI_MasterPcs5 = 5U << DSPI_MASTER_PCS_SHIFT, /*!< DSPI master transfer use PCS5 signal. */

    kDSPI_MasterPcsContinuous = 1U << 20, /*!< Indicates whether the PCS signal is continuous. */
    kDSPI_MasterActiveAfterTransfer = 1U << 21,
    /*!< Indicates whether the PCS signal is active after the last frame transfer.*/
};

#define DSPI_SLAVE_CTAR_SHIFT (0U)    /*!< DSPI slave CTAR shift macro; used internally. */
#define DSPI_SLAVE_CTAR_MASK  (0x07U) /*!< DSPI slave CTAR mask macro; used internally. */
/*! @brief Use this enumeration for the DSPI slave transfer configFlags. */
enum _dspi_transfer_config_flag_for_slave
{
    kDSPI_SlaveCtar0 = 0U << DSPI_SLAVE_CTAR_SHIFT, /*!< DSPI slave transfer use CTAR0 setting.
                                                         DSPI slave can only use PCS0. */
};

/*! @brief DSPI transfer state, which is used for DSPI transactional API state machine. */
enum _dspi_transfer_state
{
    kDSPI_Idle = 0x0U, /*!< Nothing in the transmitter/receiver. */
    kDSPI_Busy,        /*!< Transfer queue is not finished. */
    kDSPI_Error        /*!< Transfer error. */
};

/*! @brief DSPI master command date configuration used for the SPIx_PUSHR.*/
typedef struct _dspi_command_data_config
{
    bool isPcsContinuous;    /*!< Option to enable the continuous assertion of the chip select between transfers.*/
    uint8_t whichCtar;       /*!< The desired Clock and Transfer Attributes
                                                Register (CTAR) to use for CTAS.*/
    uint8_t whichPcs;        /*!< The desired PCS signal to use for the data transfer.*/
    bool isEndOfQueue;       /*!< Signals that the current transfer is the last in the queue.*/
    bool clearTransferCount; /*!< Clears the SPI Transfer Counter (SPI_TCNT) before transmission starts.*/
} dspi_command_data_config_t;

/*! @brief DSPI master ctar configuration structure.*/
typedef struct _dspi_master_ctar_config
{
    uint32_t baudRate;                /*!< Baud Rate for DSPI. */
    uint32_t bitsPerFrame;            /*!< Bits per frame, minimum 4, maximum 16.*/
    dspi_clock_polarity_t cpol;       /*!< Clock polarity. */
    dspi_clock_phase_t cpha;          /*!< Clock phase. */
    dspi_shift_direction_t direction; /*!< MSB or LSB data shift direction. */

    uint32_t pcsToSckDelayInNanoSec;     /*!< PCS to SCK delay time in nanoseconds; setting to 0 sets the minimum
                                            delay. It also sets the boundary value if out of range.*/
    uint32_t lastSckToPcsDelayInNanoSec; /*!< The last SCK to PCS delay time in nanoseconds; setting to 0 sets the
                                            minimum delay. It also sets the boundary value if out of range.*/

    uint32_t betweenTransferDelayInNanoSec;
    /*!< After the SCK delay time in nanoseconds; setting to 0 sets the minimum
        delay. It also sets the boundary value if out of range.*/
} dspi_master_ctar_config_t;

/*! @brief DSPI master configuration structure.*/
typedef struct _dspi_master_config
{
    dspi_ctar_selection_t whichCtar;      /*!< The desired CTAR to use. */
    dspi_master_ctar_config_t ctarConfig; /*!< Set the ctarConfig to the desired CTAR. */

    dspi_which_pcs_t whichPcs;                     /*!< The desired Peripheral Chip Select (pcs). */
    dspi_pcs_polarity_config_t pcsActiveHighOrLow; /*!< The desired PCS active high or low. */

    bool enableContinuousSCK;   /*!< CONT_SCKE, continuous SCK enable. Note that the continuous SCK is only
                                     supported for CPHA = 1.*/
    bool enableRxFifoOverWrite; /*!< ROOE, receive FIFO overflow overwrite enable. If ROOE = 0, the incoming
                                     data is ignored and the data from the transfer that generated the overflow
                                     is also ignored. If ROOE = 1, the incoming data is shifted to the
                                     shift register. */

    bool enableModifiedTimingFormat;        /*!< Enables a modified transfer format to be used if true.*/
    dspi_master_sample_point_t samplePoint; /*!< Controls when the module master samples SIN in the Modified Transfer
                                                 Format. It's valid only when CPHA=0. */
} dspi_master_config_t;

/*! @brief DSPI slave ctar configuration structure.*/
typedef struct _dspi_slave_ctar_config
{
    uint32_t bitsPerFrame;      /*!< Bits per frame, minimum 4, maximum 16.*/
    dspi_clock_polarity_t cpol; /*!< Clock polarity. */
    dspi_clock_phase_t cpha;    /*!< Clock phase. */
                                /*!< Slave only supports MSB and does not support LSB.*/
} dspi_slave_ctar_config_t;

/*! @brief DSPI slave configuration structure.*/
typedef struct _dspi_slave_config
{
    dspi_ctar_selection_t whichCtar;     /*!< The desired CTAR to use. */
    dspi_slave_ctar_config_t ctarConfig; /*!< Set the ctarConfig to the desired CTAR. */

    bool enableContinuousSCK;               /*!< CONT_SCKE, continuous SCK enable. Note that the continuous SCK is only
                                                 supported for CPHA = 1.*/
    bool enableRxFifoOverWrite;             /*!< ROOE, receive FIFO overflow overwrite enable. If ROOE = 0, the incoming
                                                 data is ignored and the data from the transfer that generated the overflow
                                                 is also ignored. If ROOE = 1, the incoming data is shifted to the
                                                 shift register. */
    bool enableModifiedTimingFormat;        /*!< Enables a modified transfer format to be used if true.*/
    dspi_master_sample_point_t samplePoint; /*!< Controls when the module master samples SIN in the Modified Transfer
                                               Format. It's valid only when CPHA=0. */
} dspi_slave_config_t;



/*!
 * @brief Forward declaration of the @ref _dspi_slave_handle typedefs.
 */
typedef struct _dspi_slave_handle dspi_slave_handle_t; /*!< The slave handle. */


/*!
 * @brief Completion callback function pointer type.
 *
 * @param base DSPI peripheral address.
 * @param handle Pointer to the handle for the DSPI slave.
 * @param status Success or error code describing whether the transfer completed.
 * @param userData Arbitrary pointer-dataSized value passed from the application.
 */
typedef void (*dspi_slave_transfer_callback_t)(SPI_Type* base,
    dspi_slave_handle_t* handle,
    status_t status,
    void* userData);



/*! @brief DSPI half-duplex(master) transfer structure */
typedef struct _dspi_half_duplex_transfer
{
    uint8_t* txData;      /*!< Send buffer */
    uint8_t* rxData;      /*!< Receive buffer */
    size_t txDataSize;    /*!< Transfer bytes for transmit */
    size_t rxDataSize;    /*!< Transfer bytes */
    uint32_t configFlags; /*!< Transfer configuration flags; set from @ref _dspi_transfer_config_flag_for_master. */
    bool isPcsAssertInTransfer; /*!< If Pcs pin keep assert between transmit and receive. true for assert and false for
                                   de-assert. */
    bool isTransmitFirst;       /*!< True for transmit first and false for receive first. */
} dspi_half_duplex_transfer_t;



/*! @brief DSPI slave transfer handle structure used for the transactional API. */
struct _dspi_slave_handle
{
    uint32_t bitsPerFrame;          /*!< The desired number of bits per frame. */
    volatile bool isThereExtraByte; /*!< Indicates whether there are extra bytes.*/

    uint8_t* volatile txData;                  /*!< Send buffer. */
    uint8_t* volatile rxData;                  /*!< Receive buffer. */
    volatile size_t remainingSendByteCount;    /*!< A number of bytes remaining to send.*/
    volatile size_t remainingReceiveByteCount; /*!< A number of bytes remaining to receive.*/
    size_t totalByteCount;                     /*!< A number of transfer bytes*/

    volatile uint8_t state; /*!< DSPI transfer state.*/

    volatile uint32_t errorCount; /*!< Error count for slave transfer.*/

    dspi_slave_transfer_callback_t callback; /*!< Completion callback. */
    void* userData;                          /*!< Callback user data. */
};

#endif

/*! @brief DSPI master/slave transfer structure.*/
typedef struct _dspi_transfer
{
	uint8_t* txData; /*!< Send buffer. */
	uint8_t* rxData; /*!< Receive buffer. */
	volatile size_t dataSize; /*!< Transfer bytes. */

	uint32_t configFlags; /*!< Transfer transfer configuration flags. Set from @ref
                             _dspi_transfer_config_flag_for_master if the transfer is used for master or @ref
                             _dspi_transfer_config_flag_for_slave enumeration if the transfer is used for slave.*/
} dspi_transfer_t;

/*!
 * @brief Forward declaration of the @ref _dspi_master_handle typedefs.
 */
typedef struct _dspi_master_handle dspi_master_handle_t; /*!< The master handle. */

/*!
 * @brief Completion callback function pointer type.
 *
 * @param base DSPI peripheral address.
 * @param handle Pointer to the handle for the DSPI master.
 * @param status Success or error code describing whether the transfer completed.
 * @param userData Arbitrary pointer-dataSized value passed from the application.
 */
typedef void (*dspi_master_transfer_callback_t)(SPI_TypeDef* base,
                                                dspi_master_handle_t* handle,
                                                status_t status,
                                                void* userData);

/*! @brief DSPI master transfer handle structure used for transactional API. */
struct _dspi_master_handle
{
	uint32_t bitsPerFrame; /*!< The desired number of bits per frame. */
	volatile uint32_t command; /*!< The desired data command. */
	volatile uint32_t lastCommand; /*!< The desired last data command. */

	uint8_t fifoSize; /*!< FIFO dataSize. */

	volatile bool
	isPcsActiveAfterTransfer; /*!< Indicates whether the PCS signal is active after the last frame transfer.*/
	volatile bool isThereExtraByte; /*!< Indicates whether there are extra bytes.*/

	uint8_t* volatile txData; /*!< Send buffer. */
	uint8_t* volatile rxData; /*!< Receive buffer. */
	volatile size_t remainingSendByteCount; /*!< A number of bytes remaining to send.*/
	volatile size_t remainingReceiveByteCount; /*!< A number of bytes remaining to receive.*/
	size_t totalByteCount; /*!< A number of transfer bytes*/

	volatile uint8_t state; /*!< DSPI transfer state, see @ref _dspi_transfer_state.*/

	dspi_master_transfer_callback_t callback; /*!< Completion callback. */
	void* userData; /*!< Callback user data. */
};


/**********************************************************************************************************************
 * API
 *********************************************************************************************************************/

/*!
 *@name Instance & dummy data
 * @{
 */
uint32_t SPI_GetInstance(SPI_TypeDef* base);

uint8_t SPI_GetDummyDataInstance(SPI_TypeDef* base);

void SPI_SetDummyData(SPI_TypeDef* base, uint8_t dummyData);

/*!
 *@}
*/

/*!
 * @name Initialization
 * @{
 */

status_t SPI_MasterInit(SPI_TypeDef* base, SPI_InitTypeDef* masterConfig);

status_t SPI_MasterInitWithPins(SPI_TypeDef* base, SPI_InitTypeDef* masterConfig,
                                const GPIO_Pin sck_pin,
                                const GPIO_Pin mosi_pin,
                                const GPIO_Pin miso_pin,
                                const GPIO_Pin cs_pin);

void SPI_MasterGetDefaultConfig(SPI_InitTypeDef* masterConfig);

status_t SPI_SlaveInit(SPI_TypeDef* base, SPI_InitTypeDef* slaveConfig);

status_t SPI_SlaveInitWithPins(SPI_TypeDef* base, SPI_InitTypeDef* slaveConfig,
                               const GPIO_Pin sck_pin,
                               const GPIO_Pin mosi_pin,
                               const GPIO_Pin miso_pin,
                               const GPIO_Pin cs_pin);

void SPI_SlaveGetDefaultConfig(SPI_InitTypeDef* slaveConfig);

/*!
 * @}
 */


/*!
 * @name Status
 * @{
 */

static inline uint32_t SPI_GetStatusFlags(SPI_TypeDef* base)
{
	return (base->STATR);
}

/*!
 * @brief Clears the SPI status flag.
 *
 * This function  clears the desired status bit by using a write-1-to-clear. The user passes in the base and the
 * desired status bit to clear.  The list of status bits is defined in the <b>dspi_status_and_interrupt_request_t</b>.
 * The function uses these bit positions in its algorithm to clear the desired flag state. This is an example.
 *
 * @code
 *  SPI_ClearStatusFlags(base, );
 * @endcode
 *
 * @param base DSPI peripheral address.
 * @param statusFlags The status flag used from the type dspi_flags.
 */
static inline void SPI_ClearStatusFlags(SPI_TypeDef* base, uint32_t statusFlags)
{
	base->STATR = statusFlags; /*!< The status flags are cleared by writing 1 (w1c).*/
}

/*!
*@}
 */

/*!
* @name Interrupts
* @{
*/

/*! @note Please use WCH APIs to manage SPI interrupts.**/

/*!
*@}
*/

/*!
* @name DMA Control
* @{
*/

/*! @note Please use WCH APIs to manage SPI DMA Control. This section waits for further discussion.**/

/*!
*@}
*/

/*!
* @name BUS Operations
*/

/*! @note Please use WCH APIs to manage SPI BUS Operations. This section waits for further discussion.**/


/*!
*@}
*/


/*!
 * @name Transactional APIs
 * @{
 */


/*Transactional APIs -- Master*/


#if defined(__cplusplus)
}
#endif /*_cplusplus*/
/*!
 *@}
 */

#endif //HITSIC_CH32V103_MRS_SC_SPI_H
