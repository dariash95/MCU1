/*
 * 010_Master_Tx_Testing.c
 *
 *  Created on: May 19, 2021
 *      Author: Daniela
 */

#include<stdio.h>
#include<string.h>
#include "stm32f103xx.h"

#define SLAVE_ADDR	0x68
uint8_t some_data[] = "Testing\n";

I2C_Handle_t I2C1Handle;

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}

void I2C_GPIOInits(void){

	GPIO_Handle_t I2CPins;
	I2CPins.pGPIOx = GPIOB;

	// SCL -> B6
	I2CPins.GPIO_PinConfig.GPIO_PinMode = 1; // Speed = 10 MHz
	I2CPins.GPIO_PinConfig.GPIO_Config = 3; // Alternate function Open Drain
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_6;
	GPIO_Init(&I2CPins);

	// SDA -> B7
	I2CPins.GPIO_PinConfig.GPIO_PinMode = 1; // Speed = 10 MHz
	I2CPins.GPIO_PinConfig.GPIO_Config = 3; // Alternate function Open Drain
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_7;
	GPIO_Init(&I2CPins);
}

void I2C_Inits(void){

	I2C1Handle.pI2Cx= I2C1;
	I2C1Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = 0x61; // This doesn't matter in this application bc MCU is acting like master
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTYCLYCLE_2; // Not used
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_CLK_SPEED_SM; // Standard mode

	I2C_Init(&I2C1Handle);
}

void GPIO_ButtonInit(void){
	GPIO_Handle_t gpioBtn; // Variable for the GPIO Handle

	// GPIO Button Configuration
	gpioBtn.pGPIOx = GPIOA; // Initialize variable and select port
	gpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_0;
	gpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN ;
	gpioBtn.GPIO_PinConfig.GPIO_Config = GPIO_IN_TYPE_PP;

	GPIO_Init(&gpioBtn);
}

int main (void){

	// Initialize GPIOs a IC2 pins
	I2C_GPIOInits();

	// Configure I2C
	I2C_Inits();

	// Enable I2C peripheral
	I2C_PeripheralControl(I2C1, ENABLE);

	//Configure ACK
	I2C_ManageAcking(I2C1, I2C_ACK_ENABLE);

	I2C_MasterSendData(&I2C1Handle,some_data,strlen((char*)some_data),SLAVE_ADDR);

	while (1);

}
