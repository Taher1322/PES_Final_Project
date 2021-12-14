################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/PES_Final_Project.c \
../source/UART.c \
../source/accelerometer.c \
../source/adc.c \
../source/cbfifo.c \
../source/command_line.c \
../source/command_process.c \
../source/delay.c \
../source/gpio.c \
../source/i2c.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/sysclock.c \
../source/test_cbfifo.c \
../source/test_i2c.c 

OBJS += \
./source/PES_Final_Project.o \
./source/UART.o \
./source/accelerometer.o \
./source/adc.o \
./source/cbfifo.o \
./source/command_line.o \
./source/command_process.o \
./source/delay.o \
./source/gpio.o \
./source/i2c.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/sysclock.o \
./source/test_cbfifo.o \
./source/test_i2c.o 

C_DEPS += \
./source/PES_Final_Project.d \
./source/UART.d \
./source/accelerometer.d \
./source/adc.d \
./source/cbfifo.d \
./source/command_line.d \
./source/command_process.d \
./source/delay.d \
./source/gpio.d \
./source/i2c.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/sysclock.d \
./source/test_cbfifo.d \
./source/test_i2c.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=1 -I"C:\Users\ujjai\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Final_Project\board" -I"C:\Users\ujjai\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Final_Project\source" -I"C:\Users\ujjai\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Final_Project" -I"C:\Users\ujjai\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Final_Project\drivers" -I"C:\Users\ujjai\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Final_Project\CMSIS" -I"C:\Users\ujjai\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Final_Project\utilities" -I"C:\Users\ujjai\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Final_Project\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


