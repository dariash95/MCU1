/*
 * 002_LED_Buttom.c
 *
 *  Created on: Aug 6, 2020
 *      Author: Daniela
 */

#include "stm32f103xx.h" //For the MCU
#include "stm32f1xx_gpio.h"

void delay (void){
	for(uint32_t i=0; i<500000/2; i++);
}

int main (void){

	GPIO_Handle_t gpioLED, gpioBtn; // Variable for the GPIO Handle

	// GPIO Button Configuration
	gpioBtn.pGPIOx = GPIOA; // Initialize variable and select port
	gpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_7;
	gpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN ;
	gpioBtn.GPIO_PinConfig.GPIO_Config = GPIO_IN_TYPE_PP;

	// GPIO LED Configuration
	gpioLED.pGPIOx = GPIOC; // Initialize variable and select port
	gpioLED.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_13;
	gpioLED.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT_SPEED_10;
	//gpioLED.GPIO_PinConfig.GPIO_Config = GPIO_OP_TYPE_PP;

	GPIO_PeriClkCtrl(GPIOA,ENABLE); // Clock enable for GPIO Button
	GPIO_PeriClkCtrl(GPIOC,ENABLE); // Clock enable for GPIO LED

	GPIO_Init(&gpioBtn); // GPIO Button Initialization
	GPIO_Init(&gpioLED); // GPIO LED Initialization


	while (1) {
		uint8_t value = GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_7);

		if (value == 0){
			delay(); // To prevent de-bouncing
			GPIO_ToggleOutputPin(GPIOC, GPIO_PIN_13);
		}
	}

	return 0;
}
