/*
 * stm32f1xx_gpio.h
 *
 *  Created on: Jun 5, 2020
 *      Author: Daniela
 */

// This header file contains driver specific data

#ifndef INC_STM32F1XX_GPIO_H_
#define INC_STM32F1XX_GPIO_H_

#include "stm32f103xx.h" // MCU specific header file

// Configuration structure for a GPIO Pin
typedef struct
{
	uint8_t GPIO_PinNumber; // 0-15
	uint8_t GPIO_PinMode;	// Possible values @ GPIO Pin possible modes
	uint8_t GPIO_Config;	// Possible values @ GPIO Pin possible output modes and @ GPIO Pin possible input modes
}GPIO_PinConfig_t;

// Handle structure for a GPIO Pin
typedef struct
{
	GPIO_RegDef_t *pGPIOx; // Pointer to hold the base address of the GPIO port to which the pin belongs (GPIOA, GPIOB,...)
	GPIO_PinConfig_t GPIO_PinConfig; // Holds pin configuration settings
}GPIO_Handle_t;

typedef struct
{
	EXTI_RegDef_t *pEXTIx;
}EXTI_Handle_t;

typedef struct
{
	AFIO_RegDef_t *pAFIOx;
}AFIO_Handle_t;

// GPIO Pin number
#define GPIO_PIN_0		0
#define GPIO_PIN_1		1
#define GPIO_PIN_2		2
#define GPIO_PIN_3		3
#define GPIO_PIN_4		4
#define GPIO_PIN_5		5
#define GPIO_PIN_6		6
#define GPIO_PIN_7		7
#define GPIO_PIN_8		8
#define GPIO_PIN_9		9
#define GPIO_PIN_10		10
#define GPIO_PIN_11		11
#define GPIO_PIN_12		12
#define GPIO_PIN_13		13
#define GPIO_PIN_14		14
#define GPIO_PIN_15		15

// @ GPIO Pin possible modes
#define GPIO_MODE_IN 			0 // Input
#define GPIO_MODE_OUT_SPEED_10 	1 // Output, max speed 10 MHz
#define GPIO_MODE_OUT_SPEED_2 	2 // Output, max speed 2 MHz
#define GPIO_MODE_OUT_SPEED_50 	3 // Output, max speed 50 MHz

// @ GPIO Pin possible output modes
#define GPIO_OP_TYPE_PP 		0 // General purpose push/pull
#define GPIO_OP_TYPE_OD 		1 // General purpose open drain
#define ALT_FUNC_OP_TYPE_PP 	2 // Alternate function push/pull
#define ALT_FUNC_OP_TYPE_OD 	3 // Alternate function open drain

// @ GPIO Pin possible input modes
#define GPIO_IN_TYPE_ANALOG 	0 // Analog input
#define GPIO_IN_TYPE_FLOAT		1 // Floating input
#define GPIO_IN_TYPE_PP			2 // Input with pull-up/pull-down

// Interrupt modes
#define INTER_RISING_EDGE		1 // Triggers interrupt in the rising edge
#define INTER_FALLING_EDGE		2 // Triggers interrupt in the falling edge
#define INTER_RISING_FALLING	3 // Triggers interrupt in both edges

/*					APIs Supported by this driver 					*/

// Enable/Disable peripheral clock
void GPIO_PeriClkCtrl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi);

// Initialize/De-initialize the GPIO
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx); 												// Input: GPIO Base address of the port that is going to be reset

// Data read or write
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);				// Read from pin. Output 0/1
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);									// Read from port. Output 0/1 for each pin
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value);	// Write to pin. Value = 1/0
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value);						// Write to port
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);					// Toggle GPIO pin

// Interrupt handling
void GPIO_InterHandler(GPIO_Handle_t *pGPIOHandle, EXTI_Handle_t *pEXTIHandle, AFIO_Handle_t *pAFIOHandle, uint8_t InterType);

// IQR configuration and handling
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnOrDi);									// To set IRQ Number
void GPIO_IRQPriority (uint8_t IRQNumber,uint32_t IRQPriority);							// To set the priority in IRQ
void GPIO_IRQHandling(uint8_t PinNumber);												// To process interrupt


#endif /* INC_STM32F1XX_GPIO_H_ */
