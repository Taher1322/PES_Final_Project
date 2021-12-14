/*****************************************************************************
* Copyright (C) 2021 by Taher Ujjainwala
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
 *    File name   : hexdump.c - application entry point
 *    Description : This file defines the functions to perform HEX DUMP
 *
 *    Author TAHER UJJAINWALA
 * 	  Tools : MCUXpressor IDE
 * 	  General References used in development:
 * 	  Howdy lectures slides and Alexander Dean UART section
 * 	  PES Assignment 2 - https://github.com/Taher1322/PES_Assignment_2
 * 	  PES Assignment 1 - Howdy Review
 * 	  This part of logic was discussed and implemented with Hardwardhan Singh
 * 	  DEAN UART section - https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 *
 *    Date  : 11/11/2021
 *
 *
 */


#ifndef HEXDUMP_C_
#define HEXDUMP_C_

//Including all the header files
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "cbfifo.h"
#include "MKL25Z4.h"
#include <stddef.h>
#include "UART.h"

//Defining Max Hex dump size and Number of character in single line
#define MAX_HEXDUMP_SIZE 640
#define PER_LINE_CHARACTERS 16

/*************************
 *  File name   : int_to_hexstr.c
 *  Description : Returns Hex representation of an integer.
 *	Parameters:
 *  x      Integer
 *
 * 	Returns:
 *  Hex equivalent of the integer
 *
 **************************/
static char int_to_hexchar(int x)
{
	if (x >=0 && x < 10)
		return '0' + x;
	else if
	(x >= 10 && x < 16)
		return 'A' + x - 10;
	else
		return '-';
}

//Function to calculate Hex Dump from the address specified
void hexdump(char *address, size_t nbyte) {


	  const uint8_t *buffer = (uint8_t*) address;
	  const uint8_t *threshold = (uint8_t*) address + nbyte;

	  if (nbyte > MAX_HEXDUMP_SIZE) {
		  nbyte = MAX_HEXDUMP_SIZE;
	  }

	  while(buffer < threshold )
	  {
		    putchar(int_to_hexchar(((uint32_t)(buffer) & 0xF0000000) >> 28));
			putchar(int_to_hexchar(((uint32_t)(buffer) & 0x0F000000) >> 24));
			putchar(int_to_hexchar(((uint32_t)(buffer) & 0x00F00000) >> 20));
			putchar(int_to_hexchar(((uint32_t)(buffer) & 0x000F0000) >> 16));
			putchar('_');
			putchar(int_to_hexchar(((uint32_t)(buffer) & 0x0000F000) >> 12));
			putchar(int_to_hexchar(((uint32_t)(buffer) & 0x00000F00) >>  8));
			putchar(int_to_hexchar(((uint32_t)(buffer) & 0x000000F0) >>  4));
			putchar(int_to_hexchar((uint32_t)(buffer) & 0x0000000F));
			putchar(' ');
			putchar(' ');

			for (int i=0; i < PER_LINE_CHARACTERS && buffer+i < threshold; i++)
			{
				putchar(int_to_hexchar(buffer[i] >> 4));
				putchar(int_to_hexchar(buffer[i] & 0x0f));
				putchar(' ');
			}

			buffer += PER_LINE_CHARACTERS;
			putchar('\r');
			putchar('\n');

	  }
}

#endif /* HEXDUMP_C_ */
