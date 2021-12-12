/*************************
 *
 *
 *
 *    File name   : delay.c
 *    Description : The delay is an merely approximation of the actual values that is 100ms, 500ms, 1000ms, 2000ms and 3000ms
 *    The delay function is a counter loop which runs giving appropriate delay of value as passed
 *
 *
 *    Author: TAHER S UJJAINWALA
 * 	  Tools : MCUXpressor IDE
 *    References:
 *    Date  : 09/26/2021
 *
 *
 */
//Including the Header files required for operation
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

//Delay function to generate loop as per the count value
void delay(int count)
{
	//Approximated value to get desired delay - 500ms, 1000ms, and so on
	count = count * 5200;    //Time calculated using Stop watch approximately
	//Decrementing the counter to get the desired hold- delay on further program execution
	while(count--){
		__asm volatile ("nop");
	}
}
