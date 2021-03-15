/*
 * stm32f1xx_i2c.h
 *
 *  Created on: Mar 4, 2021
 *      Author: Daniela
 */

#ifndef INC_STM32F1XX_I2C_H_
#define INC_STM32F1XX_I2C_H_

#include "stm32f103xx.h" // MCU specific header file

// Configuration structure for a I2Cx Peripheral
typedef struct{
	uint32_t I2C_SCLSpeed;
	uint8_t  I2C_DeviceAddress; // This value is mentioned by the user
	uint8_t  I2C_ACKControl;
	uint16_t I2C_FMDutyCycle; // Duty cycle of the serial clock in fast mode
}I2C_Config_t;

// Handle structure for I2Cx Peripheral
typedef struct{
	I2C_RegDef_t *pI2Cx;
	I2C_Config_t I2C_Config;
}I2C_Handle_t;

/* 							Macros  								*/
// Device speed @I2C_SCLSpeed
#define I2C_CLK_SPEED_SM		100000 // Standard Mode
#define I2C_CLK_SPEED_FM2K		200000 // Fast Mode 2K
#define I2C_CLK_SPEED_FM4K		400000 // Fast Mode 4K

// Device ACK Control @ I2C_ACKControl
#define I2C_ACK_ENABLE			1
#define I2C_ACK_DISABLE			0

// Device Duty Cycle @I2CFMDutyCycle
#define I2C_FM_DUTYCLYCLE_2		0
#define I2C_FM_DUTYCLYCLE_16_9	1

/*                 Flag related status definitions                  */
#define I2C_TXE_FLAG 			(1 << I2C_SR1_TXE)
#define I2C_RXE_FLAG			(1 << I2C_SR1_RXNE)
#define I2C_SB_FLAG				(1 << I2C_SR1_SB)
#define I2C_ADDR_FLAG			(1 << I2C_SR1_ADDR)
#define I2C_ADDR10_FLAG			(1 << I2C_SR1_ADD10)
#define I2C_BTF_FLAG			(1 << I2C_SR1_BTF)
#define I2C_STOPF_FLAG			(1 << I2C_SR1_STOPF)
#define I2C_BERR_FLAG			(1 << I2C_SR1_BERR)
#define I2C_ARLO_FLAG			(1 << I2C_SR1_ARLO)
#define I2C_AF_FLAG				(1 << I2C_SR1_AF)
#define I2C_OVR_FLAG			(1 << I2C_SR1_OVR)
#define I2C_TIMEOUT_FLAG		(1 << I2C_SR1_TIMEOUT)


/*					APIs Supported by this driver 					*/
// Enable/Disable peripheral clock
void I2C_PeriClkCtrl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);

// Initialize/De-initialize the I2C
void I2C_Init(I2C_Handle_t *pI2CxHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

// Master send data
void I2C_MasterSendData(I2C_Handle_t *pI2CxHandle, uint8_t *pTxBuffer, uint8_t length, uint8_t SlaveAddr);

// IQR configuration and handling
void I2C_IRQConfig(uint8_t IRQNumber, uint8_t EnOrDi);									// To set IRQ Number
void I2C_IRQPriority (uint8_t IRQNumber,uint32_t IRQPriority);							// To set the priority in IRQ

// Other APIs
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);

// Application callback
void I2C_ApplicationEventCallback (I2C_Handle_t *pI2CxHandle, uint8_t AppEv);

#endif /* INC_STM32F1XX_I2C_H_ */