################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
sample_app.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
sample_app.list \

OBJCOPY_BIN += \
sample_app.bin \


# All Target
all: sample_app.elf secondary-outputs

# Tool invocations
sample_app.elf: $(OBJS) $(USER_OBJS) C:\Users\Daniela\Documents\Classes\Embedded_C\MCU1\Host_MCU1\sample_app\STM32F407VGTX_FLASH.ld
	arm-none-eabi-gcc -o "sample_app.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Daniela\Documents\Classes\Embedded_C\MCU1\Host_MCU1\sample_app\STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="sample_app.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

sample_app.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "sample_app.list"
	@echo 'Finished building: $@'
	@echo ' '

sample_app.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "sample_app.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
