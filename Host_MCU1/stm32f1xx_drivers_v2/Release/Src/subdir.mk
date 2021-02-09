################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/001_LED_Toggle.c \
../Src/002_LED_Button.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/001_LED_Toggle.o \
./Src/002_LED_Button.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/001_LED_Toggle.d \
./Src/002_LED_Button.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/001_LED_Toggle.o: ../Src/001_LED_Toggle.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/001_LED_Toggle.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/002_LED_Button.o: ../Src/002_LED_Button.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/002_LED_Button.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

