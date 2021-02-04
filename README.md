# MCU1
Codes for the course Matering Microcontrollers with Embedded Driver Development

Files guide:
- stm32f103xx.h: MCU specific header file
- stm32f1xx_gpio.h: header file for GPIO driver development
- stm32f1xx_gpio.c: source file for GPIO driver development
- stm32f1xx_spi.h: header file for SPI driver developmnet
- stm32f1xx_spi.c: source file for SPI driver developmnet

Applications guide:
- 001_LED_Toggle.c: 
  For LED toggling.
  Working correctly.
 
- 002_LED_Button.c:
  Turns ON and OFF a LED using an in-board button.
  Working correctly.
 
- 003_LED_Button_ext.c:
  Turns ON and OFF a LED using an external button.
  Working correctly.
  
- 004_Button_Interrupt.c:
 Turns ON and OFF a LED using an external button using interruptions.
 Working correctly but needs confirmation.
 
- 005_SPI_Tx.c:
  Sends "Hello World" via SPI
  Not tested.
  
- 006_SPI_Tx_Arduino.c:
  Sends "Hello World" via SPI using STM32 as master and Arduino as slave.
  Working correctly but needs confirmation.
  
  

