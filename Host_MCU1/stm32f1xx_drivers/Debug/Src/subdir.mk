################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/011_Master_Rx_Testing_IT.c 

OBJS += \
./Src/011_Master_Rx_Testing_IT.o 

C_DEPS += \
./Src/011_Master_Rx_Testing_IT.d 


# Each subdirectory must supply rules for building sources it contributes
Src/011_Master_Rx_Testing_IT.o: ../Src/011_Master_Rx_Testing_IT.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -I"C:/Users/Daniela/Documents/Courses/Embedded_C/MCU1/Host_MCU1/stm32f1xx_drivers/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/011_Master_Rx_Testing_IT.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

