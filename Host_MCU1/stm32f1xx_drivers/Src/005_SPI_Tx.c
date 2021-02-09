/*
 * 005_SPI_Tx.c
 *
 *  Created on: Jan 22, 2021
 *      Author: Daniela
 */

#include "stm32f103xx.h"
#include <string.h>

/* Pin settings
 * PA4 -> SPI1_NSS
 * PA5 -> SPI1_SCLK
 * PA6 -> SPI1_MISO
 * PA7 -> SPI1_MOSI
 * Alternate function: default
 */

void SPI_GPIOInits (void){

	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOx = GPIOA;

	/*NSS -- Not used in this case
	SPIPins.GPIO_PinConfig.GPIO_PinMode = 0; // Input
	SPIPins.GPIO_PinConfig.GPIO_Config = 1; // Floating Input
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_4;
	GPIO_Init(&SPIPins); */

	// SCLK
	SPIPins.GPIO_PinConfig.GPIO_Config = 2; // Master Alternate Push Pull
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_5;
	GPIO_Init(&SPIPins);

	/*MISO -- Not used in this case
	SPIPins.GPIO_PinConfig.GPIO_PinMode = 0; // Input
	SPIPins.GPIO_PinConfig.GPIO_Config = 1; // Floating input
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_6;
	GPIO_Init(&SPIPins); */

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_Config = 2; // Master Alternate Push Pull
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_7;
	GPIO_Init(&SPIPins);
}

void SPI1_Inits (void){
	SPI_Handle_t SPI1Handle;
	SPI1Handle.pSPIx = SPI1;
	SPI1Handle.SPI_Config.SPI_BusConfig = SPI_BUS_CONFIG_FD ;
	SPI1Handle.SPI_Config.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI1Handle.SPI_Config.SPI_SCLKSpeed = SPI_SCLK_SPEED_DIV_2;
	SPI1Handle.SPI_Config.SPI_DFF = SPI_DFF_8BITS;
	SPI1Handle.SPI_Config.SPI_CPOL = SPI_CPOL_LOW;
	SPI1Handle.SPI_Config.SPI_CPHA = SPI_CPHA_LOW;
	SPI1Handle.SPI_Config.SPI_SSM = SPI_SSM_EN;

	SPI_Init(&SPI1Handle);
}

int main (void){

	char user_data[] = "Hello Word";

	SPI_GPIOInits(); // Function to initialize the GPIO pins to behave as SPI1

	SPI1_Inits(); // Function to initialize SPI1 parameters

	SPI_SSIConfig(SPI1, ENABLE); // SSI = 1. Avoids MODF errors

	SPI_PeripheralControl(SPI1, ENABLE); // Enable SPI
	// NOTE: The SPI must be enabled *after* the initialization

	SPI_SendData(SPI1, (uint8_t*)user_data, strlen(user_data));

	while(SPI_GetFlagStatus(SPI1,SPI_BUSY_FLAG));
	/* When the SPI is busy, the flag will be set and the program will hang there until the flag
	 * is down */

	SPI_PeripheralControl(SPI1, DISABLE); // Disable SPI. To close the line communication

	while(1);

	return 0;
}
