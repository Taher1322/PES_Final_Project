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
 *    File name   : test_i2c.c
 *    Description : This file defines Test's the I2C and MMA initialization
 *
 *    Author TAHER UJJAINWALA
 * 	  Tools : MCUXpressor IDE
 *
 *    Date  : 12/13/2021
 *
 *
 */

//Including all the header files
#include "MKL25Z4.h"
#include <stdio.h>
#include "i2c.h"
#include "accelerometer.h"
#include "test_i2c.h"

//This functions checks the WHO AM I value from the sensor and verifies the sensor and Initialization
void test_i2c()
{
	if(i2c_read_byte(MMA_ADDR, REG_WHOAMI) == WHOAMI)
	{
		printf("I2C Testing Done\r\n");
		printf("MMA Initialization and Verification Done\r\n");
		printf("\r\n");
	}
}
