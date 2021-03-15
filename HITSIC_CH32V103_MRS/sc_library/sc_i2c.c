//
// Created by jerry on 2021/1/15.
//

#include "sc_i2c.h"


#ifdef __cpulsplus
extern "C"{
#endif


status_t I2C_MasterRegTxBlocking(I2C_TypeDef *base, uint8_t _addr, uint32_t _reg, uint8_t _regSize, uint8_t* _data, uint32_t _dataSize)
{
    while( I2C_GetFlagStatus( base, I2C_FLAG_BUSY ) != RESET );
    I2C_GenerateSTART( base, ENABLE );

    while( !I2C_CheckEvent( base, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( base, _addr, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( base, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    if(0U != _regSize)
    {
        do
        {
            --_regSize;
            I2C_SendData(base, (uint8_t)(_reg >> (8U * _regSize)));
            while( !I2C_CheckEvent(base, I2C_EVENT_MASTER_BYTE_TRANSMITTED ));
        }while(_regSize > 0U);
    }

    while (0U != (_dataSize--))
    {
        if( I2C_GetFlagStatus( base, I2C_FLAG_TXE ) !=  RESET )
        {
            I2C_SendData(base, *_data++);
        }

        while(!I2C_CheckEvent(base, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }

    
    I2C_GenerateSTOP( base, ENABLE );
    return kStatus_Success;
}

status_t I2C_MasterRegRxBlocking(I2C_TypeDef *base, uint8_t _addr, uint32_t _reg, uint8_t _regSize, uint8_t* _data, uint32_t _dataSize)
{
    while( I2C_GetFlagStatus( base, I2C_FLAG_BUSY ) != RESET );
    I2C_GenerateSTART( base, ENABLE );

    while( !I2C_CheckEvent( base, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( base, _addr, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( base, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    if(0U != _regSize)
    {
        do
        {
            --_regSize;
            I2C_SendData(base, (uint8_t)(_reg >> (8U * _regSize)));
            while( !I2C_CheckEvent(base, I2C_EVENT_MASTER_BYTE_TRANSMITTED ));
        }while(_regSize > 0U);
    }

    I2C_GenerateSTART( base, ENABLE );

    while( !I2C_CheckEvent( base, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( base, _addr, I2C_Direction_Receiver );

    while( !I2C_CheckEvent( base, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) );

    while (0U != (_dataSize--))
    {
        if(0U == _dataSize)
        {
            I2C_AcknowledgeConfig( base, DISABLE );
        }   
        while( I2C_GetFlagStatus( base, I2C_FLAG_RXNE ) ==  RESET );  
        *_data++ = I2C_ReceiveData( base );
    }

    I2C_GenerateSTOP( base, ENABLE );

    return kStatus_Success;
}

#ifdef __cpulsplus
}
#endif

