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
 * PA4 -> SPI1_MISO
 * PA5 -> SPI1_SCLK
 * PA6 -> SPI_NSS
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
#define LED_PIN				13

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

/*** Send command #1 CMD LED CTRL. You have to send pin number and value ***/
void CMD_LED_CTRL (uint8_t commandcode){

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
		printf("Control LED executed\n");
	}
}

/*** Send command #2 CMD SENSOR READ. You have to send analog pin number ***/
void CMD_SENSOR(uint8_t commandcode){

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
	uint8_t args[1]; // args[0] = Analog pin number

	if (SPI_VerifyResponse(ackbyte)){
		// Send arguments pin number and value
		args[0] = ANALOG_PIN0;
		SPI_SendData(SPI1, args, 1);

		// Read RNXE again
		SPI_ReceiveData(SPI1, &dummyread, 1);

		// Some delay so the sensor has time to read
		delay();

		// Send dummy byte to fetch the response
		SPI_SendData(SPI1, &dummywrite, 1);

		// Read the value of the sensor
		uint8_t analog_read;
		SPI_ReceiveData(SPI1, &analog_read, 1);
		printf("Value read: %d\n", analog_read);
		printf("Sensor read executed\n");
	}
}

void CMD_LED_READ(uint8_t commandcode){

	SPI_SendData(SPI1,&commandcode, 1);

	uint8_t dummyread;
	SPI_ReceiveData(SPI1, &dummyread, 1);

	uint8_t dummywrite = 0xff;
	SPI_SendData(SPI1, &dummywrite, 1);

	uint8_t ackbyte;
	SPI_ReceiveData(SPI1, &ackbyte, 1);

	// Array of arguments
	uint8_t args[1]; // args[0] = Digital pin number

	if (SPI_VerifyResponse(ackbyte)){
		// Send arguments pin number and value
		args[0] = LED_PIN;
		SPI_SendData(SPI1, args, 1);

		// Read RNXE again
		SPI_ReceiveData(SPI1, &dummyread, 1);

		// Some delay so the sensor has time to read
		delay();

		// Send dummy byte to fetch the response
		SPI_SendData(SPI1, &dummywrite, 1);

		// Read the value of the sensor
		uint8_t led_status;
		SPI_ReceiveData(SPI1, &led_status,1);
		printf("Value read: %d\n", led_status);
		printf("Sensor LED executed\n");
	}

}

void CMD_PRINT (uint8_t commandcode){

	SPI_SendData(SPI1,&commandcode, 1);

	uint8_t dummyread;
	SPI_ReceiveData(SPI1, &dummyread, 1);

	uint8_t dummywrite = 0xff;
	SPI_SendData(SPI1, &dummywrite, 1);

	uint8_t ackbyte;
	SPI_ReceiveData(SPI1, &ackbyte, 1);

	uint8_t message[] = "Hello Word";
	uint8_t args[1];

	if (SPI_VerifyResponse(ackbyte)){
		args[0] = strlen((char*)message);

		SPI_SendData(SPI1,args,1);

		SPI_ReceiveData(SPI1,&dummyread,1);

		delay();

		for(int i = 0 ; i < args[0] ; i++){
			SPI_SendData(SPI1,&message[i],1);
			SPI_ReceiveData(SPI1,&dummyread,1);
		}
		printf("Print executed\n");
	}
}

void CMD_ID (uint8_t commandcode){

	SPI_SendData(SPI1,&commandcode, 1);

	uint8_t dummyread;
	SPI_ReceiveData(SPI1, &dummyread, 1);

	uint8_t dummywrite = 0xff;
	SPI_SendData(SPI1, &dummywrite, 1);

	uint8_t ackbyte;
	SPI_ReceiveData(SPI1, &ackbyte, 1);

	uint8_t id[11];

	if(SPI_VerifyResponse(ackbyte)){
		for (uint32_t i = 0; i<10; i++){
			SPI_SendData(SPI1, &dummywrite, 1);
			SPI_ReceiveData(SPI1, &id[i], 1);
		}
		id[10] = '\0';

		printf("ID: %d\n", id);
		printf("Print ID executed\n");
	}
}

extern void initialise_monitor_handles(void);

int main (void){

	initialise_monitor_handles();

	printf("It works!\n");

	GPIO_ButtonInit();

	SPI_GPIOInits(); // Function to initialize the GPIO pins to behave as SPI1

	SPI1_Inits(); // Function to initialize SPI1 parameters

	printf("SPI initialized\n");

	SPI_SSOEConfig(SPI1, ENABLE);

	while (1) {

		uint8_t button_value = GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0);
		// Button pressed for the 1st time
		while((button_value));
		delay();

		SPI_PeripheralControl(SPI1, ENABLE); // Enable SPI

		// CMD 1: Send command to turn ON/OFF the LED
		CMD_LED_CTRL(COMMAND_LED_CTRL);

		// Button pressed for the 2nd time
		while((button_value));
		delay();

		// CMD 2: Send command to read sensor
		CMD_SENSOR(COMMAND_SENSOR_READ);

		// Button pressed for the 3rd time
		while((button_value));
		delay();

		// CMD 3:
		CMD_LED_READ(COMMAND_LED_READ);

		// Button pressed for the 4th time
		while((button_value));
		delay();

		// CMD 4:
		CMD_PRINT(COMMAND_PRINT);

		// Button pressed for the 5th time
		while((button_value));
		delay();

		// CMD 5:
		CMD_ID(COMMAND_ID_READ);

		while(SPI_GetFlagStatus(SPI1, SPI_BUSY_FLAG));

		SPI_PeripheralControl(SPI1, DISABLE); // Disable SPI. To close the line communication
	}

	return 0;
}
