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
 *    File name   : cfifo.h - Circular Buffer functions definition
 *    Description : This file defines the functions prototype for enqueue, dequeue, length, capacity and reset. This functions are used on top of UART during the
 *    transmit and receive flow of the code
 *
 *    Author TAHER UJJAINWALA
 * 	  Tools : MCUXpressor IDE
 * 	  General References used in development:
 * 	  Howdy lectures slides and Alexander Dean UART section
 * 	  PES Assignment 2 - https://github.com/Taher1322/PES_Assignment_2
 * 	  DEAN UART section - https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 *
 *
 *    Date  : 11/09/2021
 *
 */
//Including all the header files
#ifndef CBFIFO_H_
#define CBFIFO_H_
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


//Commands used for UART implementation and a way to add more commands without changing much of code
//This Implementation Logic was discussed with Gaurang Rane to implement Circular buffer using the Enum
typedef enum fifo_buffers {
	TXD 	= 0,
	RXD 	= 1
} fifo_type;


/*
* Enqueues data onto the FIFO, up to the limit of the available FIFO
* capacity.
*
 * Parameters:
 *   buf      Pointer to the data
 *   nbyte    Max number of bytes to enqueue
 *   command_type  	Enqueue data for either Transmit or Receive
 *
 * Returns:
 *   The number of bytes actually enqueued, which could be 0. In case
 * of an error, returns -1.
 */

size_t cbfifo_enqueue(void *buf, size_t nbyte, uint32_t command_type);

/*
 * Attempts to remove ("dequeue") up to nbyte bytes of data from the
 * FIFO. Removed data will be copied into the buffer pointed to by buf.
 *
 * Parameters:
 *   buf      Destination for the dequeued data
 *   nbyte    Bytes of data requested
 *   command_type  	Enqueue data for either Transmit or Receive
 *
 * Returns:
 *   The number of bytes actually copied, which will be between 0 and
 * nbyte.
 *
 * To further explain the behavior: If the FIFO's current length is 24
 * bytes, and the caller requests 30 bytes, cbfifo_dequeue should
 * return the 24 bytes it has, and the new FIFO length will be 0. If
 * the FIFO is empty (current length is 0 bytes), a request to dequeue
 * any number of bytes will result in a return of 0 from
 * cbfifo_dequeue.
 */

size_t cbfifo_dequeue(void *buf, size_t nbyte, uint32_t command_type);

/*
 * Returns the number of bytes currently on the FIFO.
 *
 * Parameters:
 *   command_type  To calculate the length for either Transmit or Receive
 *
 * Returns:
 *   Number of bytes currently on FIFO
 */

size_t cbfifo_length(uint32_t command_type);

/*
 * Returns the FIFO's Total capacity
 *
 * Parameters:
 *   command_type  To calculate the length for either Transmit or Receive
 *
 * Returns:
 *   The capacity, in bytes, for the FIFO
 */

size_t cbfifo_capacity(uint32_t command_type);

/*
 * Resets the FIFO's parameters
 *
 * Parameters:
 *   command_type  To calculate the length for either Transmit or Receive
 *
 * Returns:
 *  none
 */

void cbfifo_reset(uint32_t command_type);

#endif /* CBFIFO_H_ */
