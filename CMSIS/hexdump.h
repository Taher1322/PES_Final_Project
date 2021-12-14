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
 *    File name   : hexdump.h -
 *    Description : This file defines the functions perform HEX DUMP
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

#ifndef HEXDUMP_H_
#define HEXDUMP_H_

/*********************************************************************************************************
 Hexdumps for provided start address and length. Length can be hex or decimal.
*
 * Parameters:
 *   loc      Starting Address
 *   nbyte    Length of Hex Dump
 *
 *
 * Returns:
 *  none
**************************************************************************************************************/
void hexdump(const void *loc, size_t nbyte);

#endif /* HEXDUMP_H_ */
