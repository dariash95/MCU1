/*
 * stm32f103xx_spi.h
 *
 *  Created on: Jan 19, 2021
 *      Author: Daniela
 */

#ifndef INC_STM32F1XX_SPI_H_
#define INC_STM32F1XX_SPI_H_

#include "stm32f103xx.h" // MCU specific header file

// Configuration structure for a SPIx Peripheral
typedef struct {
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SCLKSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
}SPI_Config_t;

// Handle structure for SPIx Peripheral
typedef struct{
	SPI_RegDef_t	*pSPIx; 	// Pointer to hold the base address of the SPIx (1,2,3)
	SPI_Config_t	SPI_Config; // Holds pin configuration settings
	uint8_t 		*pTxBuffer; // To store the app Tx Buffer address
	uint8_t 		*pRxBuffer; // To store the app Rx Buffer address
	uint8_t 		TxLen;		// To store Tx length
	uint8_t 		RxLen;		// To store Rx Length
	uint8_t 		TxState;	// To store Tx State
	uint8_t 		RxState;	// To store Rx State
}SPI_Handle_t;

/* 							Macros  								*/
// Device modes @SPI_DeviceMode BIT 2 SPI_CR1
#define	SPI_DEVICE_MODE_MASTER			1
#define	SPI_DEVICE_MODE_SLAVE			0

// Bus configuration @SPI_BusConfig BITs [15:14] and 10 SPI_CR1
#define	SPI_BUS_CONFIG_FD 				1	// Full Duplex
#define	SPI_BUS_CONFIG_HD				2	// Half Duplex
#define	SPI_BUS_CONFIG_SIMPLEX_RSONLY 	3	// Simplex, Rx Only

// Clock Speed @SPI_SCLKSpeed BITS [5:3] SPI_CR1
#define SPI_SCLK_SPEED_DIV_2			0
#define SPI_SCLK_SPEED_DIV_4			1
#define SPI_SCLK_SPEED_DIV_8			2
#define SPI_SCLK_SPEED_DIV_16			3
#define SPI_SCLK_SPEED_DIV_32			4
#define SPI_SCLK_SPEED_DIV_64			5
#define SPI_SCLK_SPEED_DIV_128			6
#define SPI_SCLK_SPEED_DIV_256			7

// Data frame format @SPI_DFF
#define SPI_DFF_8BITS					0
#define SPI_DFF_16BITS					1

// Clock polarity @SPI_CPOL
#define SPI_CPOL_HIGH					1
#define SPI_CPOL_LOW					0

// Clock phase @SPI_CPHA
#define SPI_CPHA_HIGH					1
#define SPI_CPHA_LOW					0

// Software Slave Management @SPI_SSM
#define SPI_SSM_EN						1	// Hardware management
#define SPI_SSM_DI						0	// Software management

/*                 Flag related status definitions                  */
#define SPI_TXE_FLAG 					(1 << SPI_SR_TXE)
#define SPI_RXE_FLAG					(1 << SPI_SR_RXNE)
#define SPI_BUSY_FLAG					(1 << SPI_SR_BSY)

/*                 Interrupt related definitions                    */
#define SPI_READY 						0
#define SPI_BUSY_IN_RX					1
#define SPI_BUSY_IN_TX					2

/*                Possible SPI Application Events                   */
#define SPI_EVENT_TX_COMPLETE			1
#define SPI_EVENT_RX_COMPLETE			2
#define SPI_EVENT_OVR_COMPLETE			3

/*					APIs Supported by this driver 					*/
// Enable/Disable peripheral clock
void SPI_PeriClkCtrl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

// Initialize/De-initialize the SPI
void SPI_Init(SPI_Handle_t *pSPIxHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

// Data send and receive
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len);

uint8_t SPI_SendData_Inter(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len);
uint8_t SPI_ReceiveData_Inter(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len);

// Interrupt handling
// void SPI_InterHandler(SPI_Handle_t *pSPIHandle, uint8_t InterType);

// IQR configuration and handling
void SPI_IRQConfig(uint8_t IRQNumber, uint8_t EnOrDi);									// To set IRQ Number
void SPI_IRQPriority (uint8_t IRQNumber,uint32_t IRQPriority);							// To set the priority in IRQ
void SPI_IRQHandling(SPI_Handle_t *pSPIxHandle);										// To process interrupt

// Other APIs
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseTransmission(SPI_Handle_t *pSPIxHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIxHandle);

// Application callback
void SPI_ApplicationEventCallback (SPI_Handle_t *pSPIxHandle, uint8_t AppEv);

#endif /* INC_STM32F1XX_SPI_H_ */
