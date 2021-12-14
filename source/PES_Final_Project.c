/*
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    PES_Final_Project.c
 * @brief   Application entry point.
 */
//Including all the header files
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "UART.h"
#include "sysclock.h"
#include "delay.h"
#include "gpio.h"
#include "command_line.h"
#include "command_process.h"
#include "cbfifo.h"
#include "hexdump.h"
#include "accelerometer.h"
#include "i2c.h"
#include "adc.h"
#include "test_cbfifo.h"
#include "test_i2c.h"

/* TODO: insert other definitions and declarations here. */
/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    //Enabling Port B clock
    PORTB_CLK();

    //Enabling Port D clock
    PORTD_CLK();

    //Initializing the RGB On Board LED
    INIT_LED();

    //Default LED state as OFF
    DEFAULT_LED_STATE();

    //Initializing the Read-Write Pin for RS485
    INIT_RW_PIN();

    //RGB Test Sequence
    INIT_TEST();

    //Default Read-Write Pin as Low
    REDE_OFF();

    //Initializing the ADC
    Init_ADC();

    //Initializing the UART0 for command processor
    Init_UART0(9600);

    //Initializing the UART1 for RS485 communication
    Init_UART1(9600);

    //Initializing the I2C protocol bus
    Init_I2C();

    //Initializing MMA sensor
    if (!init_mma()) {

    	while (1)
    		;
    }

    delay(100);

    //Testing the I2C - Automated test
    test_i2c();

    //Interactive print statements
    printf( "Welcome to PES Final Project \r\n");
    printf("?");


    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {

    	//Reading user input from terminal
    	serial_input();
    }
    return 0 ;
}

