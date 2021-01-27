//
// Created by jerry on 2021/1/15.
//

#include "ch32v10x_spi.h"
#include "sc_spi.h"
#include "sc_common.h"

#include "assert.h"

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static SPI_TypeDef* const s_spiBases[] = {SPI1_BASE, SPI2_BASE};


/**********************************************************************************************************************
 * Code
 *********************************************************************************************************************/

status_t SPI_MasterInit(SPI_TypeDef* base, SPI_InitTypeDef* masterConfig)
{
	/**Init APB Clock**/
	if ((SPI_TypeDef*)SPI1_BASE == base)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);
	}
	else if ((SPI_TypeDef*)SPI2_BASE == base)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | RCC_APB2Periph_AFIO, ENABLE);
	}
	else return kStatus_Fail;

	SPI_Init(base, masterConfig);
	SPI_Cmd(base, ENABLE);

	return kStatus_Success;
}

status_t SPI_MasterInitWithPins(SPI_TypeDef* base, SPI_InitTypeDef* masterConfig, 
	const GPIO_Pin sck_pin,
	const GPIO_Pin mosi_pin, 
	const GPIO_Pin miso_pin, 
	const GPIO_Pin cs_pin)
{
	/**Init APB Clock**/
	if ((SPI_TypeDef*)SPI1_BASE == base)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);
	}
	else if ((SPI_TypeDef*)SPI2_BASE == base)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | RCC_APB2Periph_AFIO, ENABLE);
	}

	/** GPIO Config **/
	GPIO_InitTypeDef gpio_in_config, gpio_out_config;

	gpio_in_config.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_out_config.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_out_config.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_QuickInit(cs_pin, kGPIO_DigitalOutput, 1, &gpio_out_config);
	if ((SPI_TypeDef*)SPI1_BASE == base)
	{
		if (A5 == sck_pin && A7 == mosi_pin && A6 == miso_pin)
		{
			GPIO_QuickInit(A5, kGPIO_DigitalOutput, 1, &gpio_out_config);
			GPIO_QuickInit(A7, kGPIO_DigitalOutput, 1, &gpio_out_config);
			GPIO_QuickInit(A6, kGPIO_DigitalInput, 1, &gpio_in_config);
		}
		else if (B3 == sck_pin && B5 == mosi_pin && B4 == miso_pin)
		{
			/** Remap needed. **/
			GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);

			GPIO_QuickInit(B3, kGPIO_DigitalOutput, 1, &gpio_out_config);
			GPIO_QuickInit(B5, kGPIO_DigitalOutput, 1, &gpio_out_config);
			GPIO_QuickInit(B4, kGPIO_DigitalInput, 1, &gpio_in_config);
		}
		else return kStatus_Fail;
	}
	else if ((SPI_TypeDef*)SPI2_BASE == base)
	{
		if (B13 == sck_pin && B15 == mosi_pin && B14 == miso_pin)
		{
			GPIO_QuickInit(B13, kGPIO_DigitalOutput, 1, &gpio_out_config);
			GPIO_QuickInit(B15, kGPIO_DigitalOutput, 1, &gpio_out_config);
			GPIO_QuickInit(B14, kGPIO_DigitalInput, 1, &gpio_in_config);
		}
		else return kStatus_Fail;
	}
	else return kStatus_Fail;
	
	SPI_Init(base, masterConfig);
	SPI_Cmd(base, ENABLE);

	return kStatus_Success;
}

void SPI_MasterGetDefaultConfig(SPI_InitTypeDef* masterConfig)
{
	masterConfig->SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	masterConfig->SPI_Mode = SPI_Mode_Master;
	masterConfig->SPI_DataSize = SPI_DataSize_8b;

	masterConfig->SPI_CPOL = SPI_CPOL_Low;                      
	masterConfig->SPI_CPHA = SPI_CPHA_1Edge;

	masterConfig->SPI_NSS = SPI_NSS_Soft;                               
	masterConfig->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;       //Default Configuration
	masterConfig->SPI_FirstBit = SPI_FirstBit_MSB;                      
	masterConfig->SPI_CRCPolynomial = 7;
}

status_t SPI_SlaveInit(SPI_TypeDef* base, SPI_InitTypeDef* slaveConfig)
{
	/**Init APB Clock**/
	if ((SPI_TypeDef*)SPI1_BASE == base)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);
	}
	else if ((SPI_TypeDef*)SPI2_BASE == base)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | RCC_APB2Periph_AFIO, ENABLE);
	}
	else return kStatus_Fail;

	SPI_Init(base, slaveConfig);
	SPI_Cmd(base, ENABLE);

	return kStatus_Success;
}

status_t SPI_SlaveInitWithPins(SPI_TypeDef* base, SPI_InitTypeDef* slaveConfig,
	const GPIO_Pin sck_pin,
	const GPIO_Pin mosi_pin,
	const GPIO_Pin miso_pin,
	const GPIO_Pin cs_pin)
{
	/**Init APB Clock**/
	if ((SPI_TypeDef*)SPI1_BASE == base)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);
	}
	else if ((SPI_TypeDef*)SPI2_BASE == base)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | RCC_APB2Periph_AFIO, ENABLE);
	}

	/** GPIO Config **/
	GPIO_InitTypeDef gpio_in_config, gpio_out_config;

	gpio_in_config.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_out_config.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_out_config.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_QuickInit(cs_pin, kGPIO_DigitalOutput, 1, &gpio_in_config);
	if ((SPI_TypeDef*)SPI1_BASE == base)
	{
		if (A5 == sck_pin && A7 == mosi_pin && A6 == miso_pin)
		{
			GPIO_QuickInit(A5, kGPIO_DigitalOutput, 1, &gpio_in_config);
			GPIO_QuickInit(A7, kGPIO_DigitalOutput, 1, &gpio_in_config);
			GPIO_QuickInit(A6, kGPIO_DigitalInput, 1, &gpio_out_config);
		}
		else if (B3 == sck_pin && B5 == mosi_pin && B4 == miso_pin)
		{
			/** Remap needed. **/
			GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);

			GPIO_QuickInit(B3, kGPIO_DigitalOutput, 1, &gpio_in_config);
			GPIO_QuickInit(B5, kGPIO_DigitalOutput, 1, &gpio_in_config);
			GPIO_QuickInit(B4, kGPIO_DigitalInput, 1, &gpio_out_config);
		}
		else return kStatus_Fail;
	}
	else if ((SPI_TypeDef*)SPI2_BASE == base)
	{
		if (B13 == sck_pin && B15 == mosi_pin && B14 == miso_pin)
		{
			GPIO_QuickInit(B13, kGPIO_DigitalOutput, 1, &gpio_in_config);
			GPIO_QuickInit(B15, kGPIO_DigitalOutput, 1, &gpio_in_config);
			GPIO_QuickInit(B14, kGPIO_DigitalInput, 1, &gpio_out_config);
		}
		else return kStatus_Fail;
	}
	else return kStatus_Fail;

	SPI_Init((SPI_TypeDef*)base, slaveConfig);
	SPI_Cmd(base, ENABLE);

	return kStatus_Success;
}

void SPI_SlaveGetDefaultConfig(SPI_InitTypeDef* slaveConfig)
{
	slaveConfig->SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	slaveConfig->SPI_Mode = SPI_Mode_Slave;
	slaveConfig->SPI_DataSize = SPI_DataSize_8b;
	
	slaveConfig->SPI_CPOL = SPI_CPOL_Low;
	slaveConfig->SPI_CPHA = SPI_CPHA_1Edge;
	
	slaveConfig->SPI_NSS = SPI_NSS_Soft;
	slaveConfig->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;       //Default Configuration
	slaveConfig->SPI_FirstBit = SPI_FirstBit_MSB;
	slaveConfig->SPI_CRCPolynomial = 7;
}
	

/*!
* @name BUS Operations
*/
	
uint32_t SPI_GetInstance(SPI_TypeDef* base)
{
	uint32_t instance;

	/* Find the instance index from base address mappings. */
	for (instance = 0; instance < ARRAY_SIZE(s_spiBases); instance++)
	{
		if (s_spiBases[instance] == base)
		{
			break;
		}
	}

	assert(instance < ARRAY_SIZE(s_spiBases));

	return instance;
}

/*!
* @name BUS Operations
*/
	
#if defined(__cplusplus)
}
#endif /*_cplusplus*/

