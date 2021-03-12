/*
 * stm32f1xx_i2c.c
 *
 *  Created on: Mar 4, 2021
 *      Author: Daniela
 */

#include"stm32f1xx_i2c.h"

uint16_t AHB1_Prescaler[8] = {2,4,8,16,64,128,256,512};
uint16_t APB1_Prescaler[4] = {2,4,8,16};

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhase(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearAddrFlag(I2C_RegDef_t *pI2Cx);
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);

/* 				Private Function Implementation 			       */

/******************************************************************
 * @func			2C_GenerateStartCondition (I2C Generate START condition)
 * @brief			This functions generates the start condition
 * @param [in]		Base Address of the I2C Peripheral
 * @return			None
 * @note 			None
 */
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx){

	pI2Cx->CR1 |= (1 << I2C_CR1_START);

}

/******************************************************************
 * @func			I2C_ExecuteAddressPhase (I2C execute address phase)
 * @brief			This functions sends the slave address via I2C and
 * 					read/write bit.
 * @param [in]		Base Address of the I2C Peripheral
 * @param [in]		Slave address
 * @return			None
 * @note 			None
 */
static void I2C_ExecuteAddressPhase(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr){

	SlaveAddr = SlaveAddr << 1;
	SlaveAddr &= ~(1); // Set 0 to write
	pI2Cx->DR = SlaveAddr;
}

/******************************************************************
 * @func			I2C_ClearAddrFlag (I2C clear ADDR flag)
 * @brief			This functions clears ADDR flag by reading SR1 and SR2
 * @param [in]		Base Address of the I2C Peripheral
 * @param [in]		Slave address
 * @return			None
 * @note 			None
 */
static void I2C_ClearAddrFlag(I2C_RegDef_t *pI2Cx){

	uint32_t dummyRead = pI2Cx->SR1;
	dummyRead = pI2Cx->SR2;
	(void) dummyRead;

}

/******************************************************************
 * @func			2C_GenerateStopCondition (I2C Generate STOP condition)
 * @brief			This functions generates the stop condition
 * @param [in]		Base Address of the I2C Peripheral
 * @return			None
 * @note 			None
 */
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx){

	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);

}

/* 					APIs Function Implementation 					*/

/******************************************************************
 * @func			I2C_PeriClkCtrl (I2C Peripheral Clock Control)
 * @brief			This functions enables or disables peripheral clock for the given I2C
 * @param [in]		Base Address of the I2C Peripheral
 * @param [in]		Enable/Disable Macros
 * @return			None
 * @note 			None
 */
void I2C_PeriClkCtrl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi){
	if (EnOrDi == ENABLE) {
		if (pI2Cx == I2C1) {
			I2C1_PCLK_EN();
		} else if (pI2Cx == I2C2) {
			I2C2_PCLK_EN();
		}
	}
	else
	{
		if (pI2Cx == I2C1) {
			I2C1_PCLK_DI();
		} else if (pI2Cx == I2C2) {
			I2C2_PCLK_DI();
		}
	}
}

/******************************************************************
 * @func			RCC_GetPCLKValue (RCC Get PCLK 1 Value)
 * @brief			This functions calculates the frequency of the APB1 clock
 * @param [in]		None
 * @return			Frequency of the clock
 * @note 			None
 */
uint32_t RCC_GetPCLK1Value (void){

	uint32_t pclk1, sysclk;

	uint8_t clksrc, temp, ahb1_prescaler, apb1_prescaler;

	/* System clock: Defined on the register CFGR of RCC
	 * Move the bits two positions to the right so they a in the position 0 and 1.
	 * Then mask and get the value */
	clksrc = (RCC ->CFGR >> 2) & 0x3;

	if (clksrc == 0){
		sysclk = 16000000;
	} else if (clksrc == 1){
		sysclk = 8000000;
	}

	// AHB Prescaler: Defined on register CFGR
	temp = (RCC ->CFGR >> 4) & 0xF;

	if (temp < 8){
		ahb1_prescaler = 1;
	} else {
		ahb1_prescaler = AHB1_Prescaler[temp-8];
		/* Example: temp = 9 -> 1001 -> [1] -> 9-8 */
	}

	// APB1 Prescaler
	temp = (RCC ->CFGR >> 8) & 0x7;

	if (temp < 4){
		apb1_prescaler = 1;
	} else {
		apb1_prescaler = APB1_Prescaler[temp-4];
	}

	pclk1 = (sysclk/ahb1_prescaler)/apb1_prescaler;

	return pclk1;
}

/******************************************************************
 * @func			I2C_Init (I2C Initialization)
 * @brief			This functions initializes a given I2C
 * @param [in]		Base Address of the I2C Handle
 * @return			None
 * @note 			None
 */
void I2C_Init(I2C_Handle_t *pI2CxHandle){

	uint32_t temp = 0;

	// Configuration of ACK bit
	temp |= (pI2CxHandle->I2C_Config.I2C_ACKControl << 10);
	pI2CxHandle->pI2Cx->CR1 = temp;

	// Configuration of the FREQ
	temp = 0;
	temp |= RCC_GetPCLK1Value()/ 1000000U;
	pI2CxHandle->pI2Cx->CR2 = (temp & 0x3F); // To mask the rest of the bits

	// Configuration of the slave address
	temp |= (pI2CxHandle->I2C_Config.I2C_DeviceAddress << 1);
	temp |= (1 << 14); // Bit 14 must be 1 according to the manual
	pI2CxHandle->pI2Cx->OAR1 = temp;

	// CCR calculations
	uint16_t ccr_value = 0;
	temp = 0;

	if(pI2CxHandle->I2C_Config.I2C_SCLSpeed <= I2C_CLK_SPEED_SM){
		// Standard mode
		ccr_value = RCC_GetPCLK1Value()/(2*pI2CxHandle->I2C_Config.I2C_SCLSpeed);
		temp |= (ccr_value & 0xFFF);
	} else{
		// Fast mode
		temp |= (1 << 15);
		temp |= (pI2CxHandle->I2C_Config.I2C_FMDutyCycle << 14);

		if (pI2CxHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTYCLYCLE_2){
			ccr_value = RCC_GetPCLK1Value()/(3*pI2CxHandle->I2C_Config.I2C_SCLSpeed);
		} else {
			ccr_value = RCC_GetPCLK1Value()/(25*pI2CxHandle->I2C_Config.I2C_SCLSpeed);
		}
		temp |= (ccr_value & 0xFFF);
	}

	pI2CxHandle->pI2Cx->CCR = temp;
}

/******************************************************************
 * @func			I2C_DeInit (I2C De-initialization)
 * @brief			This functions resets a given I2C Port
 * @param [in]		Base Address of the I2C that is going to be reset
 * @return			None
 * @note 			None
 */
void I2C_DeInit(I2C_RegDef_t *pI2Cx){
	if (pI2Cx == I2C1) {
		I2C1_REG_RESET();
	} else if (pI2Cx == I2C2) {
		I2C2_REG_RESET();
	}
}

/******************************************************************
 * @func			SPI_GetFlagStatus (SPI get flag status)
 * @brief			This functions sends data via SPI
 * @param [in]		Base Address of the SPI
 * @param [in]		Requested flag
 * @return			None
 * @note
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName){

	if(pI2Cx->SR1 & FlagName){
		return FLAG_SET; // When the bit is one in that register
	}

	return FLAG_RESET; // The programs loops in this state until it changes and the flag is set
 }

/******************************************************************
 * @func			I2C_MasterSendData (I2C Master send data)
 * @brief			This functions controls master to send data
 * @param [in]		I2C Handle
 * @param [in]		Tx Buffer
 * @param [in]		Length
 * @param [in]		Slave address
 * @return			None
 * @note 			None
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CxHandle, uint8_t *pTxBuffer, uint8_t length, uint8_t SlaveAddr){

	// Generate start condition
	I2C_GenerateStartCondition(pI2CxHandle->pI2Cx);

	// Confirm that the START generation is completed by checking the SB flag in the in the SR1 register
	while(!(I2C_GetFlagStatus(pI2CxHandle->pI2Cx, I2C_SB_FLAG)));

	// Send the address of the slave with the R/NW bit
	I2C_ExecuteAddressPhase(pI2CxHandle->pI2Cx, SlaveAddr);

	// Confirm that address phase is completed by checking the ADDR flag in the SR1 register
	while(!(I2C_GetFlagStatus(pI2CxHandle->pI2Cx, I2C_ADDR_FLAG)));

	// Clear ADDR flag
	I2C_ClearAddrFlag(pI2CxHandle->pI2Cx);

	// Send data until length = 0
	while (length > 0){
		while(!(I2C_GetFlagStatus(pI2CxHandle->pI2Cx, I2C_TXE_FLAG)));
		pI2CxHandle->pI2Cx->DR = *pTxBuffer;
		pTxBuffer ++;
		length--;
	}

	// When length = 0, Wait for TXE = 1 and BFT = 1 before generating the STOP condition
	while(!(I2C_GetFlagStatus(pI2CxHandle->pI2Cx, I2C_TXE_FLAG)));
	while(!(I2C_GetFlagStatus(pI2CxHandle->pI2Cx, I2C_BTF_FLAG)));

	// Generate STOP condition
	I2C_GenerateStopCondition(pI2CxHandle->pI2Cx);

}


/******************************************************************
 * @func			I2C_IRQConfig (I2C IRQ Configuration)
 * @brief			This functions configures the priority in the IRQ list
 * @param [in]		IQR Number
 * @param [in]		IRQ Priority
 * @param [in]		Enable or disable
 * @return			None
 * @note 			None
 */
void I2C_IRQConfig(uint8_t IRQNumber, uint8_t EnOrDi){

	if (EnOrDi == ENABLE){
			if (IRQNumber <= 31){ // IRQ Number 0-31
				*NVIC_ISER0 |= (1<< IRQNumber); // Set ISER0
			} else if (IRQNumber > 31 && IRQNumber < 64){ // IRQ Number 32-63
				*NVIC_ISER1 |= (1<< (IRQNumber%32)); // Set ISER1
			} else if (IRQNumber >= 64 && IRQNumber < 96){ // IRQ Number 64-95
				*NVIC_ISER2 |= (1<< (IRQNumber%64)); // Set ISER2
			}
		} else {
			if (IRQNumber <= 31){ // IRQ Number 0-31
				*NVIC_ICER0 |= (1<< IRQNumber); // Set ICER0
			} else if (IRQNumber > 31 && IRQNumber < 64){ // IRQ Number 32-63
				*NVIC_ICER1 |= (1<< (IRQNumber%32)); // Set ICER1
			} else if (IRQNumber >= 64 && IRQNumber < 96){ // IRQ Number 64-95
				*NVIC_ICER2 |= (1<< (IRQNumber%64)); // Set ICER2
			}
		}
}

/******************************************************************
 * @func			I2C_IRQPriority (I2C IRQ Priority)
 * @brief			This functions process the IRQ Priority
 * @param [in]		IRQ Number
 * @param [in]		IRQ Priority
 * @return			None
 * @note 			None
 */

void I2C_IRQPriority (uint8_t IRQNumber,uint32_t IRQPriority){

	uint8_t iprx =  IRQNumber/4; // Define which IPR Register you have to use (0-59)
	uint8_t iprx_section =  IRQNumber%4; // Define the section on the IPR (0-4) * 8 bc each section is 8 bits
	uint8_t aux =  ((8* iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED)); // This is the arrangement done bc the 4 first bits of each section are not implemented.

	*(NVIC_PRIO_BASEADDR + (iprx)) |= (IRQPriority <<aux);
}

/******************************************************************
 * @func			I2C_PeripheralControl (I2C Peripheral Control)
 * @brief			This functions enables/disables I2C *after* the parameters initialization
 * @param [in]		Base Address of the I2C Peripheral
 * @param [in]		Enable/Disable Macros
 * @return			None
 * @note 			None
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi){
	if (EnOrDi == ENABLE){
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
	} else {
		pI2Cx->CR1 &= ~(1 << I2C_CR1_PE);
	}
}
