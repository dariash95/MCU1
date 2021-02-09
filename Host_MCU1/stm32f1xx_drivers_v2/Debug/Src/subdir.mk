################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/004_Button_Interrupt.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/004_Button_Interrupt.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/004_Button_Interrupt.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/004_Button_Interrupt.o: ../Src/004_Button_Interrupt.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -I"C:/Users/Daniela/Documents/Courses/Embedded_C/MCU1/Host_MCU1/stm32f1xx_drivers_v2/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/004_Button_Interrupt.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -I"C:/Users/Daniela/Documents/Courses/Embedded_C/MCU1/Host_MCU1/stm32f1xx_drivers_v2/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -c -I../Inc -I"C:/Users/Daniela/Documents/Courses/Embedded_C/MCU1/Host_MCU1/stm32f1xx_drivers_v2/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

