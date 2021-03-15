//
// Created by jerry on 2021/1/15.
//

#ifndef HITSIC_CH32V103_MRS_SC_I2C_H
#define HITSIC_CH32V103_MRS_SC_I2C_H

#include "sc_common.h"

#include "ch32v10x_i2c.h"

#ifdef __cpulsplus
extern "C"{
#endif

/*!
 * @brief Performs a polling send transaction on the I2C bus.
 *
 * @param base  The I2C peripheral base pointer.
 * @param txBuff The pointer to the data to be transferred.
 * @param txSize The length in bytes of the data to be transferred.
 * @param flags Transfer control flag to decide whether need to send a stop, use kI2C_TransferDefaultFlag
 *  to issue a stop and kI2C_TransferNoStop to not send a stop.
 * @retval kStatus_Success Successfully complete the data transmission.
 * @retval kStatus_I2C_ArbitrationLost Transfer error, arbitration lost.
 * @retval kStataus_I2C_Nak Transfer error, receive NAK during transfer.
 */
status_t SCI2C_MasterWriteBlocking(I2C_TypeDef *base, const uint8_t *txBuff, size_t txSize, uint32_t flags);

/*!
 * @brief Performs a polling receive transaction on the I2C bus.
 *
 * @note The I2C_MasterReadBlocking function stops the bus before reading the final byte.
 * Without stopping the bus prior for the final read, the bus issues another read, resulting
 * in garbage data being read into the data register.
 *
 * @param base I2C peripheral base pointer.
 * @param rxBuff The pointer to the data to store the received data.
 * @param rxSize The length in bytes of the data to be received.
 * @param flags Transfer control flag to decide whether need to send a stop, use kI2C_TransferDefaultFlag
 *  to issue a stop and kI2C_TransferNoStop to not send a stop.
 * @retval kStatus_Success Successfully complete the data transmission.
 * @retval kStatus_I2C_Timeout Send stop signal failed, timeout.
 */
status_t SCI2C_MasterReadBlocking(I2C_TypeDef *base, uint8_t *rxBuff, size_t rxSize, uint32_t flags);

#ifdef __cpulsplus
}
#endif

#endif //HITSIC_CH32V103_MRS_SC_I2C_H
