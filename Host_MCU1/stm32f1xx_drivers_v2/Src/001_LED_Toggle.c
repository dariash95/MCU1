/*
 * 001_LED_Toggle.c
 *
 *  Created on: Jul 30, 2020
 *      Author: Daniela
 */

#include "stm32f103xx.h" //For the MCU
#include "stm32f1xx_gpio.h"

void delay (void){
	for(uint32_t i=0; i<500000; i++);
}

int main (void){

	GPIO_Handle_t gpioLED; // Variable for the GPIO Handle
	gpioLED.pGPIOx = GPIOC; // Initialize variable and select port
	gpioLED.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_13;
	gpioLED.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT_SPEED_10;
	//gpioLED.GPIO_PinConfig.GPIO_Config = GPIO_OP_TYPE_PP;

	GPIO_PeriClkCtrl(GPIOC,ENABLE); // Clock enable
	GPIO_Init(&gpioLED); // GPIO Initialization

	while (1) {
		GPIO_ToggleOutputPin(GPIOC, GPIO_PIN_13);
		delay();
	}

	return 0;
}
