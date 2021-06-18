################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/012_Slave_Tx_String.c 

OBJS += \
./Src/012_Slave_Tx_String.o 

C_DEPS += \
./Src/012_Slave_Tx_String.d 


# Each subdirectory must supply rules for building sources it contributes
Src/012_Slave_Tx_String.o: ../Src/012_Slave_Tx_String.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -I"C:/Users/Daniela/Documents/Courses/Embedded_C/MCU1/Host_MCU1/stm32f1xx_drivers/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/012_Slave_Tx_String.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

