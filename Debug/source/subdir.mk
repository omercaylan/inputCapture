################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/main.c \
../source/semihost_hardfault.c 

OBJS += \
./source/main.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/main.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DFRDM_K64F -DFREEDOM -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\omer-\Documents\MCUXpressoIDE_11.2.0_4120\workspace\k64f_ftm_input_capture\board" -I"C:\Users\omer-\Documents\MCUXpressoIDE_11.2.0_4120\workspace\k64f_ftm_input_capture\source" -I"C:\Users\omer-\Documents\MCUXpressoIDE_11.2.0_4120\workspace\k64f_ftm_input_capture" -I"C:\Users\omer-\Documents\MCUXpressoIDE_11.2.0_4120\workspace\k64f_ftm_input_capture\drivers" -I"C:\Users\omer-\Documents\MCUXpressoIDE_11.2.0_4120\workspace\k64f_ftm_input_capture\device" -I"C:\Users\omer-\Documents\MCUXpressoIDE_11.2.0_4120\workspace\k64f_ftm_input_capture\CMSIS" -I"C:\Users\omer-\Documents\MCUXpressoIDE_11.2.0_4120\workspace\k64f_ftm_input_capture\utilities" -I"C:\Users\omer-\Documents\MCUXpressoIDE_11.2.0_4120\workspace\k64f_ftm_input_capture\component\serial_manager" -I"C:\Users\omer-\Documents\MCUXpressoIDE_11.2.0_4120\workspace\k64f_ftm_input_capture\component\lists" -I"C:\Users\omer-\Documents\MCUXpressoIDE_11.2.0_4120\workspace\k64f_ftm_input_capture\component\uart" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


