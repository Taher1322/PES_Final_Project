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
#include "RS485.h"
#include "accelerometer.h"
#include "i2c.h"
/* TODO: insert other definitions and declarations here. */
//unsigned char voltage[] = {0x01, 0x03, 0x00, 0x63, 0x00, 0x02, 0x34, 0x15};
unsigned char voltage[] = {0x01, 0x03, 0x00, 0x63, 0x00, 0x02, 0x34, 0x15, '\0'}; //006300023415'};
char rgucTemp[100] = "\0";
volatile char input_key;

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

    //PRINTF("Hello World\n");

    sysclock_init();

    PORTB_CLK();

    PORTD_CLK();

    INIT_LED();

    DEFAULT_LED_STATE();

    INIT_RW_PIN();

    //INIT_TEST();
    REDE_OFF();


    Init_UART0(9600);

    //Init_UART1(9600);

    uart_init_howdy(9600);


//    i2c_init();																/* init i2c	*/
//    if (!init_mma()) {												/* init mma peripheral */
//    	//Control_RGB_LEDs(1, 0, 0);							/* Light red error LED */
//    	while (1)																/* not able to initialize mma */
//    		;
//    }
//
//    delay(100);
//
//    memset(rgucTemp, 0, sizeof(rgucTemp));

    //PRINTF("Size of array is %d\n\r", sizeof(voltage));
    //PRINTF("HEX string is %s\n\r", voltage);

    int size = sizeof(voltage);

    //transmit_data0(voltage, size);

    REDE_ON();
    delay(50);
    transmit_data0(voltage, size);
    delay(100);
    REDE_OFF();
    delay(500);

    //UART0_Receive_Poll();
    //read_full_xyz();

    //unsigned char buffer[] = "H"; // From UART 1";

    //transmit_data1(buffer, sizeof(buffer));


   //uint8_t c;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
    	//c = UART1_Receive_Poll();
    	if(input_key >0)
    		printf("%c\n\r",input_key);
    	//printf("%d", c);

    }
    return 0 ;
}
