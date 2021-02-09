/*
 * stm32f1xx_spi.c
 *
 *  Created on: Jan 19, 2021
 *      Author: Daniela
 */

#include"stm32f1xx_spi.h"

/* 					APIs Function Implementation 					*/

/******************************************************************
 * @func			SPI_PeriClkCtrl (SPI Peripheral Clock Control)
 * @brief			This functions enables or disables peripheral clock for the given SPI
 * @param [in]		Base Address of the SPI Peripheral
 * @param [in]		Enable/Disable Macros
 * @return			None
 * @note 			None
 */
void SPI_PeriClkCtrl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
	if (EnOrDi == ENABLE) {
		if (pSPIx == SPI1) {
			SPI1_PCLK_EN();
		} else if (pSPIx == SPI2) {
			SPI2_PCLK_EN();
		} else {
			SPI3_PCLK_EN();
		}
	}
	else
	{
		if (pSPIx == SPI1) {
			SPI1_PCLK_DI();
		} else if (pSPIx == SPI2) {
			SPI2_PCLK_DI();
		}  else {
			SPI3_PCLK_DI();
		}
	}
}

/******************************************************************
 * @func			SPI_Init (SPI Initialization)
 * @brief			This functions initializes a given SPI
 * @param [in]		Base Address of the SPI Handle
 * @return			None
 * @note 			None
 */
void SPI_Init(SPI_Handle_t *pSPIxHandle){

	//Enable clock
	SPI_PeriClkCtrl(pSPIxHandle->pSPIx, ENABLE);


	uint32_t temp = 0;

	// Configuration of the device mode
	temp |= pSPIxHandle->SPI_Config.SPI_DeviceMode << 2;

	// Configuration of the Bus
	if (pSPIxHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_FD){
		temp &= ~(1 << SPI_CR1_BIDIMODE);
	} else if (pSPIxHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_HD){
		temp |= (1 << SPI_CR1_BIDIMODE);
	} else if (pSPIxHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RSONLY){
		temp &= ~(1 << SPI_CR1_BIDIMODE);
		temp |= (1 << SPI_CR1_RXONLY);
	}

	// Configuration of the clock speed
	temp |= (pSPIxHandle->SPI_Config.SPI_SCLKSpeed << SPI_CR1_BR);

	// Configuration of the DFF
	temp |= (pSPIxHandle->SPI_Config.SPI_DFF << SPI_CR1_DFF);

	// Configuration of the CPOL
	temp |= (pSPIxHandle->SPI_Config.SPI_CPOL << SPI_CR1_CPOL);

	// Configuration of the CPHA
	temp |= (pSPIxHandle->SPI_Config.SPI_CPHA << SPI_CR1_CPHA);

	// Configuration of the SSM
	temp |= (pSPIxHandle->SPI_Config.SPI_SSM << SPI_CR1_SSM);

	pSPIxHandle->pSPIx->CR1 = temp;
}

/******************************************************************
 * @func			SPI_DeInit (SPI De-initialization)
 * @brief			This functions resets a given SPI Port
 * @param [in]		Base Address of the SPI that is going to be reset
 * @return			None
 * @note 			None
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx){
	if (pSPIx == SPI1) {
		SPI1_REG_RESET();
	} else if (pSPIx == SPI2) {
		SPI2_REG_RESET();
	} else {
		SPI3_REG_RESET();
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
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName){

	if(pSPIx->SR & FlagName){ // while(!(pSPIx->SR & (1 << 1)));
		return FLAG_SET; // When the bit TXE is one in that register
	}

	return FLAG_RESET;
 }

/******************************************************************
 * @func			SPI_SendData (SPI send data)
 * @brief			This functions sends data via SPI
 * @param [in]		Base Address of the SPI
 * @param [in]		Buffer to store the data that is going to be sent
 * @param [in]		Length of the buffer in bytes
 * @return			None
 * @note 			Blocked communication implemented. The function call will wait until all
 *  				the bytes are transmitted
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len){

	while (len > 0){
		// Wait until TXE is set -> Tx Buffer is empty
		while((SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG)) == FLAG_RESET);
		// When the expression is true and the flag is still in reset, it will wait in the while

		// Check DFF bit
		if(pSPIx->CR1 & (SPI_CR1_DFF)){
			// 16-Bit format
			// Load data into Tx Buffer
			pSPIx->DR = *((uint16_t*)pTxBuffer); // Dereference the pointer to get the data

			// Decrease the length
			len--;
			len--; // 2 bytes to decrease

			// Increment TxBuffer in order to make it point to the next data item
			(uint16_t*)pTxBuffer++;
		} else {
			// 8-Bit format
			pSPIx->DR = *pTxBuffer;
			len--;
			pTxBuffer++;
		}
	}
}


/******************************************************************
 * @func			SPI_ReceiveData (SPI receive data)
 * @brief			This functions receives data via SPI
 * @param [in]		Base Address of the SPI
 * @param [in]		Pointer to the buffer containing the data that is going to be received
 * @param [in]		Length of the buffer in bytes
 * @return			None
 * @note
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len){

}

/**********************
 * @func			InterHandling(Interrupt handling)
 * @brief			This functions configures the interrupts
 * @param [in]		Port
 * @param [in]		Type of interrupt
 * @param [in]		Enable or disable
 * @return			None
 * @note 			None
 */
void SPI_InterHandler(SPI_Handle_t *pSPIHandle, uint8_t InterType){

}

/******************************************************************
 * @func			SPI_IRQConfig (SPI IRQ Configuration)
 * @brief			This functions configures the priority in the IRQ list
 * @param [in]		IQR Number
 * @param [in]		IRQ Priority
 * @param [in]		Enable or disable
 * @return			None
 * @note 			None
 */
void SPI_IRQConfig(uint8_t IRQNumber, uint8_t EnOrDi){

}

/******************************************************************
 * @func			SPI_IRQPriority (SPI IRQ Priority)
 * @brief			This functions process the IRQ Priority
 * @param [in]		IRQ Number
 * @param [in]		IRQ Priority
 * @return			None
 * @note 			None
 */

void SPI_IRQPriority (uint8_t IRQNumber,uint32_t IRQPriority){

}

/******************************************************************
 * @func			SPI_IRQHandling (SPI IRQ Handling)
 * @brief			This functions process the interrupt
 * @param [in]		SPI handle
 * @return			None
 * @note 			None
 */
void SPI_IRQHandling(SPI_Handle_t *pSPIxHandle){

}

/******************************************************************
 * @func			SPI_PeripheralControl (SPI Peripheral Control)
 * @brief			This functions enables/disables SPI *after* the parameters initialization
 * @param [in]		Base Address of the SPI Peripheral
 * @param [in]		Enable/Disable Macros
 * @return			None
 * @note 			None
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
	if (EnOrDi == ENABLE){
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	} else {
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

/******************************************************************
 * @func			SPI_SSIConfig (SPI SSI Configuration)
 * @brief			This functions enables/disables SPI *after* the parameters initialization
 * @param [in]		Base Address of the SPI Peripheral
 * @param [in]		Enable/Disable Macros
 * @return			None
 * @note 			To avoid conflicts with the multimaster configuartion, we are going to make
 * 					SSI = 1
 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
	if (EnOrDi == ENABLE){
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	} else {
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}

/******************************************************************
 * @func			SPI_SSOEConfig (SPI SSOE Configuration)
 * @brief			This functions controls the output in the NSS Pin
 * @param [in]		Base Address of the SPI Peripheral
 * @param [in]		Enable/Disable Macros
 * @return			None
 * @note
 */
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
	if (EnOrDi == ENABLE){
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
	} else {
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
	}
}
