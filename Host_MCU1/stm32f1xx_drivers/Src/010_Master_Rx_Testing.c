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
uint8_t received_buff[32];

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

extern void initialise_monitor_handles(void);

int main (void){

	initialise_monitor_handles();
	printf("It works!\n");

	uint8_t command_code;
	uint8_t length;

	// Initialize GPIOs a IC2 pins
	I2C_GPIOInits();

	// Configure I2C
	I2C_Inits();

	// Enable I2C peripheral
	I2C_PeripheralControl(I2C1, ENABLE);

	// Enable acking after PE = 1
	I2C_ManageAcking(I2C1, I2C_ACK_ENABLE);

	while (1){
		uint8_t button_value = GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0);

		// Button pressed for the 1st time
		while((button_value));
		delay();

		//Master sends 0x51 to the slave so it knows it has to send length
		command_code = 0x51;
		I2C_MasterSendData(&I2C1Handle, &command_code, 1, SLAVE_ADDR, I2C_SR); // Repeated start

		// Slave returns length of the data and master reads it
		I2C_MasterReceiveData(&I2C1Handle, &length, 1, SLAVE_ADDR, I2C_SR); // Repeated start

		//Master sends 0x52 to the slave so it knows it has to send data
		command_code = 0x52;
		I2C_MasterSendData(&I2C1Handle, &command_code, 1, SLAVE_ADDR, I2C_SR); // Repeated start

		// Slave sends data and masters receives it
		I2C_MasterReceiveData(&I2C1Handle, received_buff, length, SLAVE_ADDR, I2C_NO_SR); // No repeated start

		received_buff[length+1] = '\0'; //Buffer needs to be terminated with the null character so we are adding it

		// Print data
		printf("Data received: %s", received_buff);
	}
}