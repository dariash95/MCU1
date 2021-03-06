/*
 * 004_Button_Interrupt.c
 *
 *  Created on: Aug 30, 2020
 *      Author: Daniela
 */

#include <string.h> // To include memset
#include "stm32f103xx.h" //For the MCU
#include "stm32f1xx_gpio.h"

void delay (void){
	for(uint32_t i=0; i<500000/2; i++);
}

int main (void){

	GPIO_Handle_t gpioLED, gpioBtn; // Variable for the GPIO Handle
	memset(&gpioLED, 0, sizeof(gpioLED)); // Set value to 0
	memset(&gpioBtn, 0, sizeof(gpioBtn)); // Set value to 0

	// GPIO Button Configuration
	gpioBtn.pGPIOx = GPIOA; // Initialize variable and select port
	gpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_7;
	gpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN ;
	gpioBtn.GPIO_PinConfig.GPIO_Config = GPIO_IN_TYPE_PP;

	// GPIO LED Configuration
	gpioLED.pGPIOx = GPIOC; // Initialize variable and select port
	gpioLED.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_13;
	gpioLED.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT_SPEED_10;
	gpioLED.GPIO_PinConfig.GPIO_Config = GPIO_OP_TYPE_PP;

	GPIO_PeriClkCtrl(GPIOA,ENABLE); // Clock enable for GPIO Button
	GPIO_PeriClkCtrl(GPIOC,ENABLE); // Clock enable for GPIO LED

	GPIO_Init(&gpioBtn); // GPIO Button Initialization
	GPIO_Init(&gpioLED); // GPIO LED Initialization

	GPIO_InterHandler(&gpioBtn, INTER_FALLING_EDGE); //Trigger Interrupt in the falling edge */

	// IRQ Configuration
	GPIO_IRQPriority(IRQ_NO_EXTI9_5, NVIC_PRIO_15);
	GPIO_IRQConfig(IRQ_NO_EXTI9_5, ENABLE);

	while (1);
}

void EXTI9_5_IRQHandler (void){
	delay();
	GPIO_IRQHandling(GPIO_PIN_7);
	GPIO_ToggleOutputPin(GPIOC, GPIO_PIN_13);
}
