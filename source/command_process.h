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
 *    File name   : command_process.h
 *    Description : This file defines the function to implement hexdump
 *
 *    Author TAHER UJJAINWALA
 * 	  Tools : MCUXpressor IDE
 * 	  General References used in development:
 * 	  Howdy lectures slides, PES Assignment 2 and Alexander Dean UART section
 * 	  PES Assignment 2 - https://github.com/Taher1322/PES_Assignment_2  (First tested to pass all Howdy Test cases and used as it is for BreakkfastSerial)
 * 	  DEAN UART section - https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 *
 *
 *    Date  : 11/09/2021
 *
 *
 */

//Including all the header files
#ifndef COMMAND_PROCESS_H_
#define COMMAND_PROCESS_H_

#include "cbfifo.h"
#include "UART.h"
#include "sysclock.h"
#include "MKL25Z4.h"
#include "core_cm0plus.h"

void process_command(char *input);

#endif /* COMMAND_PROCESS_H_ */
