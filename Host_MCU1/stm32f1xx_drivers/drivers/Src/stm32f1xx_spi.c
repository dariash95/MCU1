/*
 * stm32f1xx_spi.c
 *
 *  Created on: Jan 19, 2021
 *      Author: Daniela
 */

#include"stm32f1xx_spi.h"

/* 			  Private helpers functions	prototypes    				*/
/* These functions are private which means that only work in the scope of this file
 * The key word static denotes that they are private
 */
static void SPI_TXE_Interrupt_Handle(SPI_Handle_t *pSPIxHandle);
static void SPI_RXNE_Interrupt_Handle(SPI_Handle_t *pSPIxHandle);
static void SPI_OVR_Interrupt_Handle(SPI_Handle_t *pSPIxHandle);

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
		temp &= ~(1 << SPI_CR1_BIDIMODE); // BIDI mode clear
	} else if (pSPIxHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_HD){
		temp |= (1 << SPI_CR1_BIDIMODE);
	} else if (pSPIxHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RSONLY){
		temp &= ~(1 << SPI_CR1_BIDIMODE); // BIDI mode clear
		temp |= (1 << SPI_CR1_RXONLY); // RXONLY set
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

	pSPIxHandle->pSPIx->CR1 = temp; // Here yoo can use = bc al the bit-fields are already configured
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
		return FLAG_SET; // When the bit is one in that register
	}

	return FLAG_RESET; // The programs loops in this state until it changes and the flag is set
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
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len){

	while (len > 0){
			// Wait until RXE is set -> Tx Buffer is empty
			while((SPI_GetFlagStatus(pSPIx, SPI_RXE_FLAG)) == FLAG_RESET);
			// When the expression is true and the flag is still in reset, it will wait in the while

			// Check DFF bit
			if(pSPIx->CR1 & (SPI_CR1_DFF)){
				// 16-Bit format
				// Load data from DR to RxBuffer
				*((uint16_t*)pRxBuffer) = pSPIx->DR; // Dereference the pointer to get the data

				// Decrease the length
				len--;
				len--; // 2 bytes to decrease

				// Increment TxBuffer in order to make it point to the next data item
				(uint16_t*)pRxBuffer++;

			} else {
				// 8-Bit format
				*pRxBuffer = pSPIx->DR ;
				len--;
				pRxBuffer++;
			}
		}
}

/******************************************************************
 * @func			SPI_SendData_Inter (SPI send data using Interrupts)
 * @brief			This functions enables TXEIE to trigger the interrupt
 * @param [in]		SPI Handle
 * @param [in]		Buffer to store the data that is going to be sent
 * @param [in]		Length of the buffer in bytes
 * @return			None
 * @note 			Blocked communication implemented. The function call will wait until all
 *  				the bytes are transmitted
 */
uint8_t SPI_SendData_Inter(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len){

	uint8_t state = pSPIHandle->TxState; // Fetch state

	if (state != SPI_BUSY_IN_TX){
		// Save the Tx Buffer address and length in some global variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = len;

		// Mark the SPI state as busy in transmission so that no other code can take over the same SPI peripheral until transmission is over
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		// Enable TXEIE control bit to get an interrupt whenever TXE flag is set
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXNEIE);

		// Data transmission will be handled by the ISR code
	}

	return state;
}

/******************************************************************
 * @func			SPI_ReceiveData_Inter (SPI ReceiveData using Interrupts)
 * @brief			This functions enables RXNEIE to trigger the interrupt
 * @param [in]		SPI Handle
 * @param [in]		Pointer to the buffer containing the data that is going to be received
 * @param [in]		Length of the buffer in bytes
 * @return			None
 * @note			None blocking API
 */
uint8_t SPI_ReceiveData_Inter(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len){

	uint8_t state = pSPIHandle->RxState;

	if (state != SPI_BUSY_IN_RX){
		// Save the Tx Buffer address and length in some global variables
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = len;

		// Mark the SPI state as busy in transmission so that no other code cab take over the same SPI peripheral until transmission is over
		pSPIHandle->RxState = SPI_BUSY_IN_RX;

		// Enable RXEIE control bit to get an interrupt whenever RXE flag is set
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);

		// Data transmission will be handled by the ISR code
	}

	return state;
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
 * @func			SPI_IRQPriority (SPI IRQ Priority)
 * @brief			This functions process the IRQ Priority
 * @param [in]		IRQ Number
 * @param [in]		IRQ Priority
 * @return			None
 * @note 			None
 */

void SPI_IRQPriority (uint8_t IRQNumber,uint32_t IRQPriority){

	uint8_t iprx =  IRQNumber/4; // Define which IPR Register you have to use (0-59)
	uint8_t iprx_section =  IRQNumber%4; // Define the section on the IPR (0-4) * 8 bc each section is 8 bits
	uint8_t aux =  ((8* iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED)); // This is the arrangement done bc the 4 first bits of each section are not implemented.

	*(NVIC_PRIO_BASEADDR + (iprx)) |= (IRQPriority <<aux);
}

/******************************************************************
 * @func			SPI_IRQHandling (SPI IRQ Handling)
 * @brief			This functions process the interrupt and check what tirggers it to proceed
 * @param [in]		SPI handle
 * @return			None
 * @note 			None
 */
void SPI_IRQHandling(SPI_Handle_t *pSPIxHandle){

	// First, find out why the interrupt happened
	uint8_t temp1, temp2;

	// Check TXE
	temp1 = pSPIxHandle->pSPIx->SR & (1 << SPI_SR_TXE); // Access the TXE in SR to check the value
	// if TXE is set, temp1 = 1. If TXE is reset, temp1 = 0
	temp2 = pSPIxHandle->pSPIx->CR2 & (1 << SPI_CR2_TXNEIE);

	// If both temp1 and temp2 = 1, then the interrupt was triggered bc of TXE flag
	if (temp1 && temp2){
		// Handle TXE
		SPI_TXE_Interrupt_Handle(pSPIxHandle);
	}

	// Check RXNE
	temp1 = pSPIxHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp2 = pSPIxHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);


	// If both temp1 and temp2 = 1, then the interrupt was triggered bc of RXNE flag
	if (temp1 && temp2){
		// Handle RXNE
		SPI_RXNE_Interrupt_Handle(pSPIxHandle);
	}

	// Check OVR
	temp1 = pSPIxHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pSPIxHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);


	// If both temp1 and temp2 = 1, then the interrupt was triggered bc of OVR flag
	if (temp1 && temp2){
		// Handle RXE
		SPI_OVR_Interrupt_Handle(pSPIxHandle);
	}
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
 * @brief			This functions enables/disables SSI parameters
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

/* 			  Private helpers functions	implementation   				*/
static void SPI_TXE_Interrupt_Handle(SPI_Handle_t *pSPIxHandle){

	if(pSPIxHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)){
		pSPIxHandle->pSPIx->DR = *((uint16_t*)pSPIxHandle->pTxBuffer); // Dereference the pointer to get the data
		pSPIxHandle->TxLen--;
		pSPIxHandle->TxLen--; // 2 bytes to decrease
		(uint16_t*)pSPIxHandle->pTxBuffer++;

	} else {
		pSPIxHandle->pSPIx->DR = *pSPIxHandle->pTxBuffer;
		pSPIxHandle->TxLen--;
		pSPIxHandle->pTxBuffer++;
	}

	if (! pSPIxHandle->TxLen ) { // When Length is zero, close SPI transmission
		SPI_CloseTransmission(pSPIxHandle);
		SPI_ApplicationEventCallback(pSPIxHandle, SPI_EVENT_TX_COMPLETE);
	}
}

static void SPI_RXNE_Interrupt_Handle(SPI_Handle_t *pSPIxHandle){

	if(pSPIxHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)){
		pSPIxHandle->pSPIx->DR = *((uint16_t*)pSPIxHandle->pRxBuffer); // Dereference the pointer to get the data
		pSPIxHandle->RxLen--;
		pSPIxHandle->RxLen--; // 2 bytes to decrease
		(uint16_t*)pSPIxHandle->pRxBuffer++;

	} else {
		pSPIxHandle->pSPIx->DR = *pSPIxHandle->pRxBuffer;
		pSPIxHandle->RxLen--;
		pSPIxHandle->pRxBuffer++;
	}

	if (! pSPIxHandle->RxLen ) { // When Length is zero, close SPI transmission
		SPI_CloseReception(pSPIxHandle);
		SPI_ApplicationEventCallback(pSPIxHandle,SPI_EVENT_RX_COMPLETE);
	}
}

static void SPI_OVR_Interrupt_Handle(SPI_Handle_t *pSPIxHandle){

	// Clear OVR flag
	uint8_t temp;
	if (pSPIxHandle->TxState != SPI_BUSY_IN_TX){
		temp = pSPIxHandle->pSPIx->DR;
		temp = pSPIxHandle->pSPIx->SR;
	}

	(void)temp; // To avoid the warning that the variable is not being used

	// Inform application
	SPI_ApplicationEventCallback(pSPIxHandle,SPI_EVENT_OVR_COMPLETE);
}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx){

	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;
}

void SPI_CloseTransmission(SPI_Handle_t *pSPIxHandle){

	pSPIxHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXNEIE); // This prevents interrupts from TXE flag
	pSPIxHandle->pTxBuffer = NULL; // Reset buffers
	pSPIxHandle->TxLen = 0;
	pSPIxHandle ->TxState = SPI_READY;
}

void SPI_CloseReception(SPI_Handle_t *pSPIxHandle){

	pSPIxHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE); // This prevents interrupts from TXE flag
	pSPIxHandle->pRxBuffer = NULL;
	pSPIxHandle->RxLen = 0;
	pSPIxHandle ->RxState = SPI_READY;
}

/* In each application this function will be override according to perform some action  */
__attribute__((weak)) void SPI_ApplicationEventCallback (SPI_Handle_t *pSPIxHandle, uint8_t AppEv){
	// This is a weak implementation. The application can override this function

}
