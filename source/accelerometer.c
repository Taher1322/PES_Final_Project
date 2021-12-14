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
 *    File name   : accelerometer.c
 *    Description : This file defines the function to read the Accelerometer Values MMA8451Q
 *
 *    Author TAHER UJJAINWALA
 * 	  Tools : MCUXpressor IDE
 * 	  General References used in development:
 * 	  DEAN I2C section
 *
 *
 *    Date  : 12/13/2021
 *
 *
 */

//Including all the header files
#include <MKL25Z4.H>
#include <stdio.h>
#include <stdlib.h>
#include "accelerometer.h"
#include "i2c.h"
#include "delay.h"
#include <math.h>

//Defining variables to read the X,Y and Z cordinate values
int16_t acc_X=0, acc_Y=0, acc_Z=0;

//Defining variables to read the Roll and Pitch values
float roll=0.0, pitch=0.0;

//MMA Data Ready variable
extern uint32_t DATA_READY;



//Function to Initialize MMA sensor - Before Initializing MMA - I2C has to be enabled
int init_mma()
{
	//Define Active mode, 14 bit samples and 800Hz ODR
	i2c_write_byte(MMA_ADDR, REG_CTRL1, 0x01);
	return 1;
}

//Funtion to read the X, Y and Z cordinate values
void read_full_xyz()
{
	//Defining run time variables
	int i;
	uint8_t data[6];
	int16_t temp[3];

	//Initialize the Start sequence
	i2c_start();

	//Send device and register details
	i2c_read_setup(MMA_ADDR , REG_XHI);

	//Read five bytes in repeated mode
	for( i=0; i<5; i++)	{
		data[i] = i2c_repeated_read(0);
	}

	//Read last byte ending repeated mode
	data[i] = i2c_repeated_read(1);

	//Decoding the received values and storing in temp variable
	for ( i=0; i<3; i++ ) {
		temp[i] = (int16_t) ((data[2*i]<<8) | data[2*i+1]);
	}

	//Align the data for 14 bits
	acc_X = temp[0]/4;
	acc_Y = temp[1]/4;
	acc_Z = temp[2]/4;

	//Print Accelerometer values
	printf("Accelerometer Values are as follows %d, %d, %d\n\r", acc_X, acc_Y, acc_Z);
}

//Funtions to single read the X, Y and Z cordinate values
void read_xyz(void)
{
	//Reads the values which is in 16 bit unsigned - So Type  casting to signed function
	acc_X = (int8_t) i2c_read_byte(MMA_ADDR, REG_XHI);
	delay(100);
	acc_Y = (int8_t) i2c_read_byte(MMA_ADDR, REG_YHI);
	delay(100);
	acc_Z = (int8_t) i2c_read_byte(MMA_ADDR, REG_ZHI);

	//Print Accelerometer values
	printf("Accelerometer Values are as follows %d, %d, %d\n\r", acc_X, acc_Y, acc_Z);

}


//Funtion to calculate Roll and Pitch values based on the X,Y and Z values
void convert_xyz_to_roll_pitch(void) {

	//Converting Roll and Pitch from X,Y, and Z values
	float ax = acc_X/COUNTS_PER_G,
				ay = acc_Y/COUNTS_PER_G,
				az = acc_Z/COUNTS_PER_G;

	//Atan returns the values in radians which will be between -PI t0 PI
	roll = atan2(ay, az)*180/M_PI;
	pitch = atan2(ax, sqrt(ay*ay + az*az))*180/M_PI;

	//Print Accelerometer values
	printf("Roll and Pitch values are as follows %f, %f\n\r", roll, pitch);

}
