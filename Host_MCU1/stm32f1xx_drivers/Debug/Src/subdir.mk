################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/007_SPI_Command_Handling.c 

OBJS += \
./Src/007_SPI_Command_Handling.o 

C_DEPS += \
./Src/007_SPI_Command_Handling.d 


# Each subdirectory must supply rules for building sources it contributes
Src/007_SPI_Command_Handling.o: ../Src/007_SPI_Command_Handling.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -I"C:/Users/Daniela/Documents/Courses/Embedded_C/MCU1/Host_MCU1/stm32f1xx_drivers/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/007_SPI_Command_Handling.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

