/*
 * main.c
 *
 *  Created on: Aug 28, 2020
 *      Author: Daniela
 */

#include "stm32f103xx.h" //For the MCU
#include "stm32f1xx_gpio.h"

int main (void){
	return 0;
}

void EXTI0_IRQHandler(void){
	GPIO_IRQHandling(0);
}
