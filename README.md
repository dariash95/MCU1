# MCU1
Codes for the course Matering Microcontrollers with Embedded Driver Development

Files guide:
- stm32f103xx.h: MCU specific header file.
- stm32f1xx_gpio.h: header file for GPIO driver development.
- stm32f1xx_gpio.c: source file for GPIO driver development.
- stm32f1xx_spi.h: header file for SPI driver development.
- stm32f1xx_spi.c: source file for SPI driver development.

Applications guide:
- 001_LED_Toggle.c: 
  - For LED toggling.
  - Working correctly.
 
- 002_LED_Button.c:
  - Turns ON and OFF a LED using an in-board button.
  - Working correctly.
 
- 003_LED_Button_ext.c:
  - Turns ON and OFF a LED using an external button.
  - Working correctly.
  
- 004_Button_Interrupt.c:
  - Turns ON and OFF a LED using an external button using interruptions.
  - Working correctly but needs confirmation.
 
- 005_SPI_Tx.c:
  - Sends "Hello World" via SPI
  - Working correctly.
  
- 006_SPI_Tx_Arduino.c:
  - Sends "Hello World" via SPI using STM32 as master and Arduino as slave.
  - Working correctly.
    
- 007_SPI_Command_Handling.c:
  - Sends a group of commands to the Arduino and it responds accordingly.
    - 1: LED control
    - 2: Sensor read
    - 3: LED status read
    - 4: Print message
    - 5: Arduinon id
  - Working correctly.

- 008_SPI_Interrupts.c:
  - Sends a message to the Arduino implementing interrupts.
  - Not working. Stays in a infinite loop when trying to send the message. Cause is unknown. 

- 009_I2C_Master_Tx_Testing.c:
  - Sends a message to the Arduino via I2C.
  - Not working. Stays in a infinite loop when trying to send the message. Master mode is not activated due to some issue with the microcontroller.
  - Issue can be fixed following procedure in the errata but "Ain't nobody got time for that".

- 010_I2C_Master_Rx_Testing.c:
  - Receives a message from the Arduino via I2C.
  - Not tested.
 
- 011_I2C_Master_Tx_Testing_IT.c:
  - Sends and receives a message from the Arduino via I2C.
  - Not tested.

- 012_I2C_Slave_Tx_String.c:
  - MCU acts as slave and Arduino acts as master.
  - A message is sent from MCU and retrieved by the Arduino.
  - Not tested.
  
