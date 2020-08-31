/*
 * stm32f1xx.h
 *
 *  Created on: May 28, 2020
 *      Author: Daniela
 */

// This header file contains MCU specific data

#ifndef INC_STM32F103XX_H_
#define INC_STM32F103XX_H_

#include <stdint.h>

/*********************** Processor specific macros ******************************/

/* ARM Cortex-MX Processor NVIC ISERx Register Addresses. Taken from ARM Cortex-MX User Guide*/
#define NVIC_ISER0				((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1				((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2				((volatile uint32_t*)0xE000E108)
#define NVIC_ISER3				((volatile uint32_t*)0xE000E10C)

/* ARM Cortex-MX Processor NVIC ICERx Register Addresses */
#define NVIC_ICER0				((volatile uint32_t*)0xE000E180)
#define NVIC_ICER1				((volatile uint32_t*)0XE000E184)
#define NVIC_ICER2				((volatile uint32_t*)0XE000E188)
#define NVIC_ICER3				/* ARM Cortex-MX Processor NVIC ICERx Register Addresses */

/* ARM Cortex-MX Processor Priority Register Addresses Calculation */
#define NVIC_PRIO_BASEADDR		((volatile uint32_t*)0xE000E400)

/* ARM Cortex-MX Processor number of priority bits implemented in the Priority Register  */
#define NO_PR_BITS_IMPLEMENTED	4

/**************************** MCU specific macros ***********************************/

/* Base address of Flash and SRAM Memories */
#define FLASH_BASEADDR			0x08000000U 	// Base memory for Flash memory
#define SRAM1_BASEADDR			0x20000000U 	// Base memory for SRAM1 memory
#define ROM_BASEADDR			0x1FFFF000U 	// Base memory for ROM memory
#define SRAM 					SRAM1_BASEADDR  // Base memory for SRAM = SRAM1 memory

/* AHB1 and APBx Bus Peripherals base addresses */
#define PERIPH_BASEADDR			0X40000000U
#define APB1PERIPH_BASEADDR		PERIPH_BASEADDR	// Base memory for APB1
#define APB2PERIPH_BASEADDR		0X40010000U		// Base memory for APB2
#define AHB1PERIPH_BASEADDR		0X40018000U		// Base memory for AHB1

/* Base addresses of peripherals hanging on AHB1 */
#define RCC_BASEADDR			0x40021000U // Base address for RCC

/* Base addresses of peripherals hanging on APB1 */
#define SPI2_BASEADDR 			(APB1PERIPH_BASEADDR + 0x3800) // Base address for SPI2
#define SPI3_BASEADDR 			(APB1PERIPH_BASEADDR + 0x3C00) // Base address for SPI3

#define I2C1_BASEADDR 			(APB1PERIPH_BASEADDR + 0x5400) // Base address for I2C1
#define I2C2_BASEADDR 			(APB1PERIPH_BASEADDR + 0x5800) // Base address for I2C2

#define UART4_BASEADDR 			(APB1PERIPH_BASEADDR + 0x4C00) // Base address for UART4
#define UART5_BASEADDR 			(APB1PERIPH_BASEADDR + 0x5000) // Base address for UART5

#define USART2_BASEADDR 		(APB1PERIPH_BASEADDR + 0x4400) // Base address for USART2
#define USART3_BASEADDR 		(APB1PERIPH_BASEADDR + 0x4800) // Base address for USART3

/* Base addresses of peripherals hanging on APB2 */
#define GPIOA_BASEADDR 			(APB2PERIPH_BASEADDR + 0x0800) // Base address for GPIOA
#define GPIOB_BASEADDR 			(APB2PERIPH_BASEADDR + 0x0C00) // Base address for GPIOB
#define GPIOC_BASEADDR 			(APB2PERIPH_BASEADDR + 0x1000) // Base address for GPIOC
#define GPIOD_BASEADDR 			(APB2PERIPH_BASEADDR + 0x1400) // Base address for GPIOD
#define GPIOE_BASEADDR 			(APB2PERIPH_BASEADDR + 0x1800) // Base address for GPIOE
#define GPIOF_BASEADDR 			(APB2PERIPH_BASEADDR + 0x1C00) // Base address for GPIOF
#define GPIOG_BASEADDR 			(APB2PERIPH_BASEADDR + 0x2000) // Base address for GPIOG

#define SPI1_BASEADDR 			(APB2PERIPH_BASEADDR + 0x3000) // Base address for SPI1

#define EXTI_BASEADDR 			(APB2PERIPH_BASEADDR + 0x0400) // Base address for EXTI

#define USART1_BASEADDR 		(APB2PERIPH_BASEADDR + 0x3800) // Base address for USART1

#define AFIO_BASEADDR			(APB2PERIPH_BASEADDR + 0x0000) // Base address for AFIO

/* Peripherals registers definitions structures */
typedef struct
{
	volatile uint32_t CRL;		// Configuration Register Low 	Offset 0x0000
	volatile uint32_t CRH;		// Configuration Register High 	Offset 0x0004
	volatile uint32_t IDR;		// Input Data Register 			Offset 0x0008
	volatile uint32_t ODR;		// Output Data Register 		Offset 0x000C
	volatile uint32_t BSRR;		// Bit Set/Reset Register		Offset 0x0010
	volatile uint32_t BRR;		// Bit Reset Register			Offset 0x0014
	volatile uint32_t LCKR;		// Configuration Lock Register	Offset 0x0018
}GPIO_RegDef_t;

typedef struct
{
	volatile uint32_t EVCR;		// Event Control Register 		Offset 0x0000
	volatile uint32_t MAPR;		// Remap and debug Register		Offset 0x0004
	volatile uint32_t EXTICR[4];// Ext inter config register 	Offset 0x0008
	volatile uint32_t MAPR2;	// Remap and debug Register	2	Offset 0x001C
}AFIO_RegDef_t;

/* RCC registers definitions structures */
typedef struct
{
	volatile uint32_t CR;		// Clock Control Register 					Offset 0x0000
	volatile uint32_t CFGR;		// Clock Config Register 					Offset 0x0004
	volatile uint32_t CIR;		// Clock Interrupt Register 				Offset 0x0008
	volatile uint32_t APB2RSTR;	// APB2 Peripheral Reset Register			Offset 0x000C
	volatile uint32_t APB1RSTR;	// APB1 Peripheral Reset Register			Offset 0x0010
	volatile uint32_t AHBENR;	// AHB Peripheral Clock Enable Register		Offset 0x0014
	volatile uint32_t APB2ENR;	// APB2 Peripheral Clock Enable Register	Offset 0x0018
	volatile uint32_t APB1ENR;	// APB1 Peripheral Clock Enable Register	Offset 0x001C
	volatile uint32_t BDCR;		// Backup Domain Control Register 			Offset 0x0020
	volatile uint32_t CSR;		// Control/Status Register					Offset 0x0024
	volatile uint32_t AHBSTR;	// AHB Peripheral Clock Reset Register		Offset 0x0028
	volatile uint32_t CFGR2;	// Clock Configuration Register 2			Offset 0x002C
}RCC_RegDef_t;

/* EXTI registers definitions structures */
typedef struct
{
	volatile uint32_t IMR;		// Interrupt mask Register 					Offset 0x0000
	volatile uint32_t EMR;		// Event mask Register 						Offset 0x0004
	volatile uint32_t RTSR;		// Rising Trigger Selection Register 		Offset 0x0008
	volatile uint32_t FTSR;		// Falling Trigger Selection Register		Offset 0x000C
	volatile uint32_t SWIER;	// Software Interrupt Event Register		Offset 0x0010
	volatile uint32_t PR;		// Pending Register							Offset 0x0014
}EXTI_RegDef_t;

/* Peripherals Definitions: Peripheral base address typecasted to GPIO_RegDef_t */
#define GPIOA						((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB						((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC						((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD						((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE						((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF						((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG						((GPIO_RegDef_t*)GPIOG_BASEADDR)

#define AFIO						((AFIO_RegDef_t*)AFIO_BASEADDR)

#define RCC							((RCC_RegDef_t*)RCC_BASEADDR)

#define EXTI 						((EXTI_RegDef_t*)EXTI_BASEADDR)

/* Clock enable macros for GPIO peripherals */
#define GPIOA_PCLK_EN()				(RCC->APB2ENR |=(1 << 2)) // Bit 2 to enable RCC for port A
#define GPIOB_PCLK_EN()				(RCC->APB2ENR |=(1 << 3)) // Bit 3 to enable RCC for port B
#define GPIOC_PCLK_EN()				(RCC->APB2ENR |=(1 << 4)) // Bit 4 to enable RCC for port C
#define GPIOD_PCLK_EN()				(RCC->APB2ENR |=(1 << 5)) // Bit 5 to enable RCC for port D
#define GPIOE_PCLK_EN()				(RCC->APB2ENR |=(1 << 6)) // Bit 6 to enable RCC for port E
#define GPIOF_PCLK_EN()				(RCC->APB2ENR |=(1 << 7)) // Bit 7 to enable RCC for port F
#define GPIOG_PCLK_EN()				(RCC->APB2ENR |=(1 << 8)) // Bit 8 to enable RCC for port G

/* Clock enable macros for AFIO peripherals */
#define AFIO_PCLK_EN()				(RCC->APB2ENR |=(1 << 0)) // Bit 0 to enable RCC for AFIO

/* Clock enable macros for I2C peripherals */
#define I2C1_PCLK_EN()				(RCC->APB1ENR |=(1 << 21)) // Bit 21 to enable RCC for I2C1
#define I2C2_PCLK_EN()				(RCC->APB1ENR |=(1 << 22)) // Bit 22 to enable RCC for I2C2

/* Clock enable macros for SPI peripherals */
#define SPI1_PCLK_EN()				(RCC->APB2ENR |=(1 << 12)) // Bit 12 to enable RCC for I2C1
#define SPI2_PCLK_EN()				(RCC->APB1ENR |=(1 << 14)) // Bit 14 to enable RCC for SPI2
#define SPI3_PCLK_EN()				(RCC->APB1ENR |=(1 << 15)) // Bit 15 to enable RCC for SPI3

/* Clock enable macros for USART peripherals */
#define USART1_PCLK_EN()			(RCC->APB2ENR |=(1 << 14)) // Bit 12 to enable RCC for USART1
#define USART2_PCLK_EN()			(RCC->APB1ENR |=(1 << 17)) // Bit 12 to enable RCC for USART2
#define USART3_PCLK_EN()			(RCC->APB1ENR |=(1 << 18)) // Bit 12 to enable RCC for USART3

/* Clock disable macros for GPIO peripherals */
#define GPIOA_PCLK_DI()				(RCC->APB2ENR &= ~(1 << 2)) // Bit 2 to disable RCC for port A
#define GPIOB_PCLK_DI()				(RCC->APB2ENR &= ~(1 << 3)) // Bit 3 to disable RCC for port B
#define GPIOC_PCLK_DI()				(RCC->APB2ENR &= ~(1 << 4)) // Bit 4 to disable RCC for port C
#define GPIOD_PCLK_DI()				(RCC->APB2ENR &= ~(1 << 5)) // Bit 5 to disable RCC for port D
#define GPIOE_PCLK_DI()				(RCC->APB2ENR &= ~(1 << 6)) // Bit 6 to disable RCC for port E
#define GPIOF_PCLK_DI()				(RCC->APB2ENR &= ~(1 << 7)) // Bit 7 to disable RCC for port F
#define GPIOG_PCLK_DI()				(RCC->APB2ENR &= ~(1 << 8)) // Bit 8 to disable RCC for port G

/* Clock disable macros for AFIO peripherals */
#define AFIO_PCLK_DI()				(RCC->APB2ENR &= ~(1 << 0)) // Bit 0 to disable RCC for AFIO

/* Clock enable macros for I2C peripherals */
#define I2C1_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 21)) // Bit 21 to disable RCC for I2C1
#define I2C2_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 22)) // Bit 22 to disable RCC for I2C2

/* Clock enable macros for SPI peripherals */
#define SPI1_PCLK_DI()				(RCC->APB2ENR &= ~(1 << 12)) // Bit 12 to disable RCC for I2C1
#define SPI2_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 14)) // Bit 14 to disable RCC for SPI2
#define SPI3_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 15)) // Bit 15 to disable RCC for SPI3

/* Clock enable macros for USART peripherals */
#define USART1_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 14)) // Bit 12 to disable RCC for USART1
#define USART2_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 17)) // Bit 12 to disable RCC for USART2
#define USART3_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 18)) // Bit 12 to disable RCC for USART3

/* Macros to reset GPIOx Peripherals */
#define GPIOA_REG_RESET()			do {(RCC->APB2RSTR|=(1 << 3)); (RCC->APB2RSTR &= ~(1 << 3));} while (0) // To execute more than one instruction per line
#define GPIOB_REG_RESET()			do {(RCC->APB2RSTR|=(1 << 4)); (RCC->APB2RSTR &= ~(1 << 4));} while (0)
#define GPIOC_REG_RESET()			do {(RCC->APB2RSTR|=(1 << 5)); (RCC->APB2RSTR &= ~(1 << 5));} while (0)
#define GPIOD_REG_RESET()			do {(RCC->APB2RSTR|=(1 << 6)); (RCC->APB2RSTR &= ~(1 << 6));} while (0)
#define GPIOE_REG_RESET()			do {(RCC->APB2RSTR|=(1 << 7)); (RCC->APB2RSTR &= ~(1 << 7));} while (0)
#define GPIOF_REG_RESET()			do {(RCC->APB2RSTR|=(1 << 8)); (RCC->APB2RSTR &= ~(1 << 8));} while (0)
#define GPIOG_REG_RESET()			do {(RCC->APB2RSTR|=(1 << 9)); (RCC->APB2RSTR &= ~(1 << 9));} while (0)

/* Macro to get a portcode given GPIOx base address*/ //If x==GPIOA, then return 0, else
#define GPIO_BASEADDR_TO_CODE(x)	((x == GPIOA) ? 0 :\
									 (x == GPIOB) ? 1 :\
									 (x == GPIOC) ? 2 :\
									 (x == GPIOD) ? 3 :\
									 (x == GPIOE) ? 4 :\
									 (x == GPIOF) ? 5 :\
									 (x == GPIOG) ? 6 :0)

/* IRQ Numbers */
#define IRQ_NO_EXTI0		6
#define IRQ_NO_EXTI1		7
#define IRQ_NO_EXTI2		8
#define IRQ_NO_EXTI3		9
#define IRQ_NO_EXTI4		10
#define IRQ_NO_EXTI9_5		23
#define IRQ_NO_EXTI15_10	40

/* IRQ Priority */
#define NVIC_PRIO_0			0
#define NVIC_PRIO_1			1
#define NVIC_PRIO_2			2
#define NVIC_PRIO_3			3
#define NVIC_PRIO_4			4
#define NVIC_PRIO_5			5
#define NVIC_PRIO_6			6
#define NVIC_PRIO_7			7
#define NVIC_PRIO_8			8
#define NVIC_PRIO_9			9
#define NVIC_PRIO_10		10
#define NVIC_PRIO_11		11
#define NVIC_PRIO_12		12
#define NVIC_PRIO_13		13
#define NVIC_PRIO_14		14
#define NVIC_PRIO_15		15

/* Some generic macros */
#define ENABLE 			1
#define DISABLE 		0
#define SET 			ENABLE
#define RESET 			DISABLE
#define GPIO_PIN_SET	ENABLE
#define GPIO_PIN_RESET	DISABLE

#include "stm32f1xx_gpio.h"

#endif /* INC_STM32F103XX_H_ */
