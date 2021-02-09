/*
 * 007_SPI_Command_Handling.c
 *
 *  Created on: Feb 9, 2021
 *      Author: Daniela
 */

#include "stm32f103xx.h"
#include <string.h>
#include <stdio.h>

/* Pin settings
 * PA4 -> SPI1_NSS
 * PA5 -> SPI1_SCLK
 * PA6 -> SPI1_MISO
 * PA7 -> SPI1_MOSI
 * Alternate function: default
 */

/*                          SPECIFIC MACROS FOR THIS API                                  */
// COMMAND CODES
#define COMMAND_LED_CTRL	0x50 // This is the command code that the slave recognizes
#define COMMAND_SENSOR_READ 0x51
#define COMMAND_LED_READ	0x52
#define COMMAND_PRINT		0x53
#define COMMAND_ID_READ		0x54

#define LED_ON 				1
#define LED_OFF				0

// Arduino Analog Pins
#define ANALOG_PIN0			0
#define ANALOG_PIN1			1
#define ANALOG_PIN2			2
#define ANALOG_PIN3			3
#define ANALOG_PIN4			4

// Arduino LED
#define LED_PIN				9

/*                                     FUNCTIONS                                          */
void delay (void){
	for(uint32_t i=0; i<500000/2; i++);
}

void SPI_GPIOInits (void){

	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOx = GPIOA;

	// NSS -- Not used in this case
	SPIPins.GPIO_PinConfig.GPIO_PinMode = 1; // Input
	SPIPins.GPIO_PinConfig.GPIO_Config = 2; // Floating Input
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_4;
	GPIO_Init(&SPIPins);

	// SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinMode = 1;
	SPIPins.GPIO_PinConfig.GPIO_Config = 2; // Master Alternate Push Pull
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_5;
	GPIO_Init(&SPIPins);

	// MISO
	SPIPins.GPIO_PinConfig.GPIO_PinMode = 0; // Input
	SPIPins.GPIO_PinConfig.GPIO_Config = 1; // Floating input
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_6;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinMode = 1;
	SPIPins.GPIO_PinConfig.GPIO_Config = 2; // Master Alternate Push Pull
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_7;
	GPIO_Init(&SPIPins);
}

void SPI1_Inits(void){

	SPI_Handle_t SPI1Handle;
	SPI1Handle.pSPIx = SPI1;
	SPI1Handle.SPI_Config.SPI_BusConfig = SPI_BUS_CONFIG_FD ;
	SPI1Handle.SPI_Config.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI1Handle.SPI_Config.SPI_SCLKSpeed = SPI_SCLK_SPEED_DIV_8;
	SPI1Handle.SPI_Config.SPI_DFF = SPI_DFF_8BITS;
	SPI1Handle.SPI_Config.SPI_CPOL = SPI_CPOL_LOW;
	SPI1Handle.SPI_Config.SPI_CPHA = SPI_CPHA_LOW;
	SPI1Handle.SPI_Config.SPI_SSM = SPI_SSM_DI;

	SPI_Init(&SPI1Handle);
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

uint8_t SPI_VerifyResponse (uint8_t ackbyte){

	if (ackbyte == 0xf5){
		// Acknowledged
		return 1;
	} else {
		// Not acknowledged
		return 0;
	}
}

int main (void){

	GPIO_ButtonInit();

	SPI_GPIOInits(); // Function to initialize the GPIO pins to behave as SPI1

	SPI1_Inits(); // Function to initialize SPI1 parameters

	SPI_SSOEConfig(SPI1, ENABLE);

	while (1) {

		uint8_t button_value = GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0);

		while(!(button_value));
		/* When the button is pressed, the statement will be 1 and negated will be zero
		 * and will come out of the loop and continue to run the program */

		delay();

		SPI_PeripheralControl(SPI1, ENABLE); // Enable SPI
		// NOTE: The SPI must be enabled *after* the initialization

		// Send command #1 CMD LED CTRL. You have to send pin number and value
		uint8_t commandcode = COMMAND_LED_CTRL;
		SPI_SendData(SPI1, &commandcode, 1);

		// Read the dummy to clear off RXNE
		uint8_t dummyread;
		SPI_ReceiveData(SPI1, &dummyread, 1);

		// Send some dummy byte to fetch the response from the slave (ACK or NACK)
		uint8_t dummywrite = 0xff;
		SPI_SendData(SPI1, &dummywrite, 1);

		// Read the response from the slave
		uint8_t ackbyte;
		SPI_ReceiveData(SPI1, &ackbyte, 1);

		// Array of arguments
		uint8_t args[2]; // args[0] = Pin number, args[1] = Value

		if (SPI_VerifyResponse(ackbyte)){
			// Send arguments pin number and value
			args[0] = LED_PIN;
			args[1] = LED_ON;
			SPI_SendData(SPI1, args, 2);
		}

		while(SPI_GetFlagStatus(SPI1, SPI_BUSY_FLAG));
		/* When the SPI is busy, the flag will be set and the program will hang there until the flag
		 * is down */

		SPI_PeripheralControl(SPI1, DISABLE); // Disable SPI. To close the line communication
	}

	return 0;
}