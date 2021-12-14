################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/hexdump.c \
../CMSIS/system_MKL25Z4.c 

OBJS += \
./CMSIS/hexdump.o \
./CMSIS/system_MKL25Z4.o 

C_DEPS += \
./CMSIS/hexdump.d \
./CMSIS/system_MKL25Z4.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=1 -I"C:\Users\ujjai\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Final_Project\board" -I"C:\Users\ujjai\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Final_Project\source" -I"C:\Users\ujjai\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Final_Project" -I"C:\Users\ujjai\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Final_Project\drivers" -I"C:\Users\ujjai\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Final_Project\CMSIS" -I"C:\Users\ujjai\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Final_Project\utilities" -I"C:\Users\ujjai\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Final_Project\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


