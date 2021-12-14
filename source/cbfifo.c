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
 *    File name   : cfifo.c - Circular Buffer functions
 *    Description : This file defines the functions for enqueue, dequeue, length, capacity and reset. This functions are used on top of UART during the
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
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "cbfifo.h"
#include "MKL25Z4.h"

//Maximum size of buffer
#define SIZE_OF_BUFFER      (1024)

//Defining structure to keep a track of head, tail, size variables
typedef struct circular_buffer{
	uint8_t  Buffer[SIZE_OF_BUFFER];
	uint32_t buffer_length;
	uint32_t read_index;
	uint32_t write_index;
}cbfifo;

cbfifo fifo[2];


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

size_t cbfifo_enqueue(void *buf, size_t nbyte, uint32_t command_type)
{
	uint32_t masking;

	if(buf == NULL)
	{
		return -1;
	}
	else
	{
		if(fifo[command_type].buffer_length >= SIZE_OF_BUFFER)
		{
			return 0;
		}
		else
		{
			int i;
			if (nbyte>=SIZE_OF_BUFFER- fifo[command_type].buffer_length)
			{
				nbyte = SIZE_OF_BUFFER- fifo[command_type].buffer_length;
			}
			else
			{

			}

			masking = __get_PRIMASK();

			__disable_irq();

			for(i = 0; i<nbyte; i++)
			{
				fifo[command_type].Buffer[fifo[command_type].write_index]= *(uint8_t*)buf;
				fifo[command_type].buffer_length++;
				fifo[command_type].write_index++;
				buf =  (uint8_t*)buf +1;
				if(fifo[command_type].write_index == SIZE_OF_BUFFER)
				{
					fifo[command_type].write_index = 0;
				}
			}

			__set_PRIMASK(masking);

			return i;
		}
	}
}

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


size_t cbfifo_dequeue(void *buf, size_t nbyte, uint32_t command_type)
{
	uint32_t masking;

	if(buf ==NULL)
	{
		return -1;
	}
	else if(fifo[command_type].buffer_length == 0)
	{
		return 0;
	}
	else
	{
		int i = 0;
		if(nbyte>fifo[command_type].buffer_length)
		{
			nbyte = fifo[command_type].buffer_length;
		}
		else
		{

		}

		masking = __get_PRIMASK();

		__disable_irq();

		for(i=0; i<nbyte; i++)
		{
			*(uint8_t*)buf=  fifo[command_type].Buffer[fifo[command_type].read_index];
			fifo[command_type].read_index++;
			fifo[command_type].buffer_length--;
			buf =  (uint8_t*)buf +1;
			if(fifo[command_type].read_index == SIZE_OF_BUFFER)
			{
				fifo[command_type].read_index = 0;
			}
		}

		__set_PRIMASK(masking);
		return i;
	}
}

/*
 * Returns the number of bytes currently on the FIFO.
 *
 * Parameters:
 *   command_type  To calculate the length for either Transmit or Receive
 *
 * Returns:
 *   Number of bytes currently on FIFO
 */

size_t cbfifo_length(uint32_t command_type)
{
    return fifo[command_type].buffer_length;
}

/*
 * Returns the FIFO's Total capacity
 *
 * Parameters:
 *   command_type  To calculate the length for either Transmit or Receive
 *
 * Returns:
 *   The capacity, in bytes, for the FIFO
 */

size_t cbfifo_capacity(uint32_t command_type)
{
    return SIZE_OF_BUFFER;
}

/*
 * Resets the FIFO's parameters
 *
 * Parameters:
 *   command_type  To calculate the length for either Transmit or Receive
 *
 * Returns:
 *  none
 */

void cbfifo_reset(uint32_t command_type)
{
	fifo[command_type].buffer_length = 0;
	fifo[command_type].read_index = 0;
	fifo[command_type].write_index = 0;
}
