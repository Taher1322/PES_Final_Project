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
 *    File name   : UART.h
 *    Description : This file defines all the functions defined in header file to Initialize UART0, Ties the getchar(), sscanf() with sysread(),
 *    also ties printf() and putchar() with syswrite()
 *
 *    Author TAHER UJJAINWALA
 * 	  Tools : MCUXpressor IDE
 * 	  General References used in development:
 * 	  Howdy lectures slides and Alexander Dean UART section
 * 	  PES Assignment 2 - https://github.com/Taher1322/PES_Assignment_2
 * 	  DEAN UART section - https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 *
 *    Date  : 11/09/2021
 *
 *
 */


#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <MKL25Z4.H>

/********************************************************************************************************
 * void Init_UART0(uint32_t baud_rate): This function Initializes UART0
 *
 * @param Baud Rate: Takes Input baud rate and initialized the UART
 *
 * @returns void
 *
 * Baud rate is calculated using the following formula
 *
 * Baud rate = Baud Rate = Baud Clock / ((OSR+1) * BR)
 *
 * where OSR is Sampling rate we have used OSR=16
 *
 * Baud clock is defined here as 24Mhz
 *
 * BR : is calculated depending on baud rate and is stored in BDL and BDH (lower and higher register)
 ***********************************************************************************************************/
void Init_UART0(uint32_t baud_rate);

/********************************************************************************************************
 * void Init_UART1(uint32_t baud_rate): This function Initializes UART1
 *
 * @param Baud Rate: Takes Input baud rate and initialized the UART
 *
 * @returns void
 *
 * Baud rate is calculated using the following formula
 *
 * Baud rate = Baud Rate = Baud Clock / ((OSR+1) * BR)
 *
 * where OSR is Sampling rate we have used OSR=16
 *
 * Baud clock is defined here as 24Mhz
 *
 * BR : is calculated depending on baud rate and is stored in BDL and BDH (lower and higher register)
 ***********************************************************************************************************/
void Init_UART1(uint32_t baud_rate);

/*************************************************************************************************************
 *
 * Name :		  	void transmit_data1(unsigned char *pdata, int total_character);
 *
 *
 * Description :	This function is used to transmit the request frames with 16bit CRC to TTL-RS485 converter
 *
 *
 * Inputs: Request Frame and Size of Request frame
 *
 * Return: NONE
 *************************************************************************************************************/
void transmit_data1(unsigned char *pdata, int total_character);

/*************************************************************************************************************
 *
 * Name :		  	int modbus_read(int i)
 *
 *
 * Description :	Funtion to read modbus data, deocde and return the values
 *
 *
 * Inputs:		Frame parameter selection
 *
 * Return: Response value
 *************************************************************************************************************/
int modbus_read(int i);

#endif /* UART_H_ */
