################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/Src/stm32f1xx_gpio.c \
../drivers/Src/stm32f1xx_i2c.c \
../drivers/Src/stm32f1xx_spi.c 

OBJS += \
./drivers/Src/stm32f1xx_gpio.o \
./drivers/Src/stm32f1xx_i2c.o \
./drivers/Src/stm32f1xx_spi.o 

C_DEPS += \
./drivers/Src/stm32f1xx_gpio.d \
./drivers/Src/stm32f1xx_i2c.d \
./drivers/Src/stm32f1xx_spi.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/Src/stm32f1xx_gpio.o: ../drivers/Src/stm32f1xx_gpio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -I"C:/Users/Daniela/Documents/Courses/Embedded_C/MCU1/Host_MCU1/stm32f1xx_drivers/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/Src/stm32f1xx_gpio.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
drivers/Src/stm32f1xx_i2c.o: ../drivers/Src/stm32f1xx_i2c.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -I"C:/Users/Daniela/Documents/Courses/Embedded_C/MCU1/Host_MCU1/stm32f1xx_drivers/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/Src/stm32f1xx_i2c.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
drivers/Src/stm32f1xx_spi.o: ../drivers/Src/stm32f1xx_spi.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -I"C:/Users/Daniela/Documents/Courses/Embedded_C/MCU1/Host_MCU1/stm32f1xx_drivers/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/Src/stm32f1xx_spi.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

