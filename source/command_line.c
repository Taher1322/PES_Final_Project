/*****************************************************************************
* Copyright (C) 2021 by Taher Ujjainwala and Howdy Pierce
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Taher Ujjainwala and the University of Colorado are not liable for
* any misuse of this material.
*
**************************************************************************/


/*************************
 *
 *
 *
 *    File name   : command_line.c
 *    Description : This file defines the function to implement continuous read from serial terminal
 *
 *    Author TAHER UJJAINWALA
 * 	  Tools : MCUXpressor IDE
 * 	  General References used in development:
 * 	  Howdy lectures slides, PES Assignment 2 and Alexander Dean UART section
 * 	  PES Assignment 2 - https://github.com/Taher1322/PES_Assignment_2  (First tested to pass all Howdy Test cases and used as it is for BreakkfastSerial)
 * 	  DEAN UART section - https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 *
 *
 *    Date  : 11/11/2021
 *
 *
 */

//Including all the header files
#include "command_line.h"
#include "uart.h"
#include "cbfifo.h"
#include <string.h>
#include <stdio.h>
#include "command_process.h"

//Adding defines
#define SERIAL_READ_BUFFER     		(2000)				//Character array value
#define ENTER_KEY_VALUE 		 	(0X0D)				//Hex character for Enter Key
#define BACKSPACE_KEY_VALUE      	(0X08)				//Hex character for Backspace Key

//Function to read user input in an Infinite Loop
void serial_input()
{

	char buffer[SERIAL_READ_BUFFER];
	char ch;
	int i = 0;

	//Infinite loop to read the character on serial terminal
	while(1)
	{
		while((cbfifo_length(RXD)==0)){			//Waiting for the receive event
			;
		}
		cbfifo_dequeue(&ch, 1, RXD);			//Reading the received character and echo on terminal
		putchar(ch);							//Echo

		if(ch == ENTER_KEY_VALUE) 				//Check if user pressed enter
		{
			buffer[i]='\0'; 					//Append the string if user pressed enter (End string character)
			break;
		}
		else if(ch == BACKSPACE_KEY_VALUE)		//Check if user pressed backspace - delete from the terminal and date buffer
		{
			i--;
			printf(" \b");						//Backspace character value
		}
		else
		{
			buffer[i] =  ch;					//Store the received data one by one in character array
			i++;								//Increment the locations in character array
		}

	}

	printf("\n\r");								//New line
	process_command(buffer);					//Process the character further and implement the functions as per user input

}

