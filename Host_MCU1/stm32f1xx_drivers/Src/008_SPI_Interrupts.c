/*
 * 008_SPI_Interrupts.c
 *
 *  Created on: Feb 16, 2021
 *      Author: Daniela
 *
 *  EXERCISE
 *  The STM32 (master) receives a message from the Arduino (slave) over SPI
 *  - User enters a message using the Arduino serial port.
 *  - Arduino notifies the STM32 about message availability.
 *  - STM32 reads and prints the message.
 *  - Uses interrupts.
 *
 */

#include<stdio.h>
#include<string.h>
#include "stm32f103xx.h"

SPI_Handle_t SPI1Handle;

#define MAX_LEN 500

char RcvBuff[MAX_LEN];

volatile char ReadByte;

volatile uint8_t rcvStop = 0;

/*This flag will be set in the interrupt handler of the Arduino interrupt GPIO */
volatile uint8_t dataAvailable = 0;

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}


void SPI_GPIOInits(void){

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

void SPI_Inits(void){

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


/*This function configures the gpio pin over which SPI peripheral issues data available interrupt */
void Slave_GPIO_InterruptPinInit(void){

	GPIO_Handle_t gpioBtn; // Variable for the GPIO Handle
	memset(&gpioBtn, 0, sizeof(gpioBtn)); // Set value to 0

	// GPIO Button Configuration
	gpioBtn.pGPIOx = GPIOA; // Initialize variable and select port
	gpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_7;
	gpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN ;
	gpioBtn.GPIO_PinConfig.GPIO_Config = GPIO_IN_TYPE_PP;
	GPIO_Init(&gpioBtn);

	// Button interrupt configuration
	EXTI_Handle_t BtnInter;
	BtnInter.pEXTIx = EXTI;

	AFIO_Handle_t AFIOBtn;
	AFIOBtn.pAFIOx = AFIO;
	GPIO_InterHandler(&gpioBtn, &BtnInter, &AFIOBtn, INTER_FALLING_EDGE); //Trigger Interrupt in the falling edge

	GPIO_IRQPriority(IRQ_NO_EXTI9_5, NVIC_PRIO_15);
	GPIO_IRQConfig(IRQ_NO_EXTI9_5, ENABLE);
}


int main(void){

	uint8_t dummy = 0xff;

	Slave_GPIO_InterruptPinInit(); // Initializes PD6 to deliver the interrupt

	//this function is used to initialize the GPIO pins to behave as SPI2 pins
	SPI_GPIOInits();

	//This function is used to initialize the SPI2 peripheral parameters
	SPI_Inits();

	/*
	* making SSOE 1 does NSS output enable.
	* The NSS pin is automatically managed by the hardware.
	* i.e when SPE=1 , NSS will be pulled to low
	* and NSS pin will be high when SPE=0
	*/
	SPI_SSOEConfig(SPI1,ENABLE);

	SPI_IRQConfig(IRQ_NO_SPI1,ENABLE);

	while(1){

		rcvStop = 0;

		while(!dataAvailable); //wait till data available interrupt from transmitter device(slave)

		GPIO_IRQConfig(IRQ_NO_EXTI9_5,DISABLE);

		//enable the SPI1 peripheral
		SPI_PeripheralControl(SPI1,ENABLE);


		while(!rcvStop)
		{
			/* fetch the data from the SPI peripheral byte by byte in interrupt mode */
			while (SPI_SendData_Inter(&SPI1Handle, &dummy, 1) == SPI_BUSY_IN_TX);
			while (SPI_ReceiveData_Inter(&SPI1Handle, &ReadByte, 1) == SPI_BUSY_IN_RX );
		}


		// confirm SPI is not busy
		while( SPI_GetFlagStatus(SPI1,SPI_BUSY_FLAG) );

		//Disable the SPI2 peripheral
		SPI_PeripheralControl(SPI1,DISABLE);

		printf("Rcvd data = %s\n",RcvBuff);

		dataAvailable = 0;

		GPIO_IRQConfig(IRQ_NO_EXTI9_5,ENABLE);
	}

	return 0;

}

/* Runs when a data byte is received from the peripheral over SPI*/
void SPI1_IRQHandler(void)
{

	SPI_IRQHandling(&SPI1Handle);
}

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEv)
{
	static uint32_t i = 0;
	/* In the RX complete event , copy data in to rcv buffer . '\0' indicates end of message(rcvStop = 1) */
	if(AppEv == SPI_EVENT_RX_COMPLETE)
	{
				RcvBuff[i++] = ReadByte;
				if(ReadByte == '\0' || ( i == MAX_LEN)){
					rcvStop = 1;
					RcvBuff[i-1] = '\0';
					i = 0;
				}
	}

}

/* Slave data available interrupt handler */
void EXTI9_5_IRQHandler(void)
{
	GPIO_IRQHandling(GPIO_PIN_6);
	dataAvailable = 1;
}