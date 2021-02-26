/*
 * stm32f1xx_gpio.c
 *
 *  Created on: Jun 5, 2020
 *      Author: Daniela
 */

#include"stm32f1xx_gpio.h"

/* 					APIs Function Implementation 					*/

/******************************************************************
 * @func			GPIO_PeriClkCtrl (GPIO Peripheral Clock Control)
 * @brief			This functions enables or disables peripheral clock for the given GPIO Port
 * @param [in]		Base Address of the GPIO Peripheral
 * @param [in]		Enable/Disable Macros
 * @return			None
 * @note 			None
 */
void GPIO_PeriClkCtrl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE) {
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_EN();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_EN();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_EN();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLK_EN();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLK_EN();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLK_EN();
		} else {
			GPIOG_PCLK_EN();
		}
	}
	else
	{
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_DI();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_DI();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_DI();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLK_DI();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLK_DI();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLK_DI();
		} else {
			GPIOG_PCLK_DI();
		}
	}
}

/******************************************************************
 * @func			GPIO_Init (GPIO Initialization)
 * @brief			This functions initializes a given GPIO Port
 * @param [in]		Base Address of the GPIO Handle
 * @return			None
 * @note 			None
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	//Enable the peripheral clock
	GPIO_PeriClkCtrl(pGPIOHandle->pGPIOx, ENABLE);

	// Configuration of the pin mode
	uint32_t temp_mode = 0;
	uint32_t temp_config = 0;

	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber <= 7){ // Configuration of Pins 0-7

		uint8_t aux = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
		uint8_t value_mode = pGPIOHandle->GPIO_PinConfig.GPIO_PinMode;
		uint8_t value_config = pGPIOHandle->GPIO_PinConfig.GPIO_Config;
		uint8_t positions_mode = 4*aux;
		uint8_t positions_config = (2 + (4 * aux));
		uint32_t clear_mode = (0x03 << positions_mode);
		uint32_t clear_config = (0x03 << positions_config);

		temp_mode = value_mode << positions_mode;
		pGPIOHandle->pGPIOx->CRL &= ~(clear_mode); // Clearing
		pGPIOHandle->pGPIOx->CRL |= temp_mode; // Setting. CAREFUL: Use | to change just the position of the pin number

		temp_mode = 0;

		temp_config = value_config << positions_config;
		pGPIOHandle->pGPIOx->CRL &= ~(clear_config); //Clearing
		pGPIOHandle->pGPIOx->CRL |= temp_config; //Setting

		temp_config = 0;

	} else { // Configuration of Pins 8-15

		uint8_t aux = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber-8;
		uint8_t value_mode = pGPIOHandle->GPIO_PinConfig.GPIO_PinMode;
		uint8_t value_config = pGPIOHandle->GPIO_PinConfig.GPIO_Config;
		uint8_t positions_mode = 4*aux;
		uint8_t positions_config = (2 + (4 * aux));
		uint32_t clear_mode = ~(0x03 << positions_mode);
		uint32_t clear_config = ~(0x03 << positions_config);

		temp_mode = value_mode << positions_mode;
		pGPIOHandle->pGPIOx->CRH &= clear_mode; // Clearing
		pGPIOHandle->pGPIOx->CRH |= temp_mode; //Setting

		temp_mode = 0;

		temp_config = value_config << positions_config;
		pGPIOHandle->pGPIOx->CRH &= clear_config; //Clearing
		pGPIOHandle->pGPIOx->CRH |= temp_config; // Setting

		temp_config = 0;
	}

	// Configuration of the Alternate Functions
	if ((pGPIOHandle->GPIO_PinConfig.GPIO_PinMode = ALT_FUNC_OP_TYPE_PP) | (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode = ALT_FUNC_OP_TYPE_OD)){

	}
}

/******************************************************************
 * @func			GPIO_DeInit (GPIO De-initialization)
 * @brief			This functions resets a given GPIO Port
 * @param [in]		Base Address of the GPIO port that is going to be reset
 * @return			None
 * @note 			None
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if (pGPIOx == GPIOA) {
		GPIOA_REG_RESET();
	} else if (pGPIOx == GPIOB) {
		GPIOB_REG_RESET();
	} else if (pGPIOx == GPIOC) {
		GPIOC_REG_RESET();
	} else if (pGPIOx == GPIOD) {
		GPIOD_REG_RESET();
	} else if (pGPIOx == GPIOE) {
		GPIOE_REG_RESET();
	} else if (pGPIOx == GPIOF) {
		GPIOF_REG_RESET();
	} else {
		GPIOG_REG_RESET();
	}
}

// Interrupt handling
/**********************
 * @func			InterHandling(Interrupt handling)
 * @brief			This functions configures the interrupts
 * @param [in]		Port
 * @param [in]		Type of interrupt
 * @param [in]		Enable or disable
 * @return			None
 * @note 			None
 */
void GPIO_InterHandler(GPIO_Handle_t *pGPIOHandle, uint8_t InterType){

	uint8_t positions = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;

	if (InterType == 1){ //Rising edge detection
		EXTI->RTSR |= (1 << positions);
		EXTI->FTSR &= ~(1 << positions); // Disable falling edge

	} else if (InterType ==2) { //Falling edge detection
		EXTI->FTSR |= (1 << positions);
		EXTI->RTSR &= ~(1 << positions); // Disable rising edge

	} else { //Detection for both edges
		EXTI->FTSR |= (1 << positions);
		EXTI->RTSR |= (1 << positions);
	}

	// Configure the GPIO port selection in AFIO_EXTICR
	uint8_t temp1 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber/4); // To define the EXTI (0-15)
	uint8_t temp2 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber%4); // To define the position in the register
	uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
	uint8_t aux = temp2*4;

	AFIO_PCLK_EN(); // RCC enable for AFIO
	AFIO->EXTICR[temp1] = portcode << aux;

	// Enable the EXTI Interrupt delivery using IMR
	EXTI->IMR |= (1 << positions);
}

// Data read or write
/******************************************************************
 * @func			PIO_ReadFromInputPin (GPIO Read from Input Pin)
 * @brief			This functions reads from an input pin
 * @param [in]		Base Address of the GPIO port in which the pin is
 * @param [in]		Pin number
 * @return			Read value 0 or 1
 * @note 			None
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t) ((pGPIOx->IDR >> PinNumber) & 0x00000001);
	return value;
}

/******************************************************************
 * @func			PIO_ReadFromInputPort (GPIO Read from Input Port)
 * @brief			This functions reads from an input port
 * @param [in]		Base Address of the GPIO port that is going to be read
 * @return			Read value 0 or 1
 * @note 			uint16_t bc there are 15 pins in a port that are going to
 * 					return 15 values of 0 or 1
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;
	value = (uint16_t) (pGPIOx->IDR);
	return value;
}

/******************************************************************
 * @func			GPIO_WriteToOutputPin (GPIO Write to output pin)
 * @brief			This functions writes a value to a pin
 * @param [in]		Base Address of the GPIO port in which the pin is
 * @param [in]		Pin number
 * @param [in]		Value that is going to be written (0 or 1)
 * @return			None
 * @note 			None
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value)
{
	if (value == GPIO_PIN_SET){
		pGPIOx->ODR |= (1 << PinNumber);
	} else {
		pGPIOx->ODR &= ~(1 << PinNumber);
	}
}

/******************************************************************
 * @func			GPIO_WriteToOutputPor (GPIO Write to output port)
 * @brief			This functions writes a value to a port
 * @param [in]		Base Address of the GPIO port
 * @param [in]		Value that is going to be written
 * @return			None
 * @note 			uint16_t bc there are going to be written 15 values for
 * 					each pin of the port
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value)
{
	pGPIOx->ODR = value;
}

/******************************************************************
 * @func			GPIO_ToggleOutputPin (GPIO Toggle pin)
 * @brief			This functions toggles a specific pin
 * @param [in]		Base Address of the GPIO port in which the pin is
 * @param [in]		Pin number
 * @return			None
 * @note 			None
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR = pGPIOx->ODR ^ (1 << PinNumber); // Same as: pGPIOx->ODR ^= (1 << PinNumber) ^ => XOR
}


// IQR configuration and handling
/******************************************************************
 * @func			GPIO_IRQConfig (GPIO IRQ Configuration)
 * @brief			This functions configures the priority in the IRQ list
 * @param [in]		IQR Number
 * @param [in]		IRQ Priority
 * @param [in]		Enable or disable
 * @return			None
 * @note 			None
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnOrDi)
{
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
 * @func			GPIO_IRQPriority (GPIO IRQ Priority)
 * @brief			This functions process the IRQ Priority
 * @param [in]		IRQ Number
 * @param [in]		IRQ Priority
 * @return			None
 * @note 			None
 */
void GPIO_IRQPriority (uint8_t IRQNumber,uint32_t IRQPriority){

	uint8_t iprx =  IRQNumber/4; // Define which IPR Register you have to use (0-59)
	uint8_t iprx_section =  IRQNumber%4; // Define the section on the IPR (0-4) * 8 bc each section is 8 bits
	uint8_t aux =  ((8* iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED)); // This is the arrangement done bc the 4 first bits of each section are not implemented.

	*(NVIC_PRIO_BASEADDR + (iprx)) |= (IRQPriority <<aux);
}

/******************************************************************
 * @func			GPIO_IRQHandling (GPIO IRQ Handling)
 * @brief			This functions process the interrupt
 * @param [in]		Pin Number
 * @return			None
 * @note 			None
 */
void GPIO_IRQHandling(uint8_t PinNumber)
{
	// Clear the EXTI Pending Register Corresponding to the Pin Number
	if (EXTI->PR & (1 << PinNumber)){
		EXTI->PR |= (1 << PinNumber);
	}
}

