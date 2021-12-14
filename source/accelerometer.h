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

#ifndef MMA8451_H
#define MMA8451_H

//Including header files
#include <stdint.h>

//MMA slave address
#define MMA_ADDR 0x3A

//Defining macros for X, Y and Z axis (High and Low)
#define REG_XHI 0x01
#define REG_XLO 0x02
#define REG_YHI 0x03
#define REG_YLO 0x04
#define REG_ZHI	0x05
#define REG_ZLO 0x06

//Defining Control and WHOAMI macros
#define REG_WHOAMI 0x0D
#define REG_CTRL1  0x2A
#define REG_CTRL4  0x2D
#define WHOAMI 0x1A

//Roll and Pitch calculation macros
#define COUNTS_PER_G (4096.0)
#define M_PI (3.14159265)


/*************************************************************************************************************
 *
 * Name :		  	int init_mma()
 *
 *
 *
 * Description :	Function to Initialize MMA sensor - Before Initializing MMA - I2C has to be enabled
 *
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
int init_mma(void);

/*************************************************************************************************************
 *
 * Name :		  	void read_full_xyz()
 *
 *
 *
 * Description :	Funtion to read the X, Y and Z cordinate values
 *
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void read_full_xyz(void);

/*************************************************************************************************************
 *
 * Name :		  	void read_xyz()
 *
 *
 *
 * Description :	Funtions to single read the X, Y and Z cordinate values
 *
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void read_xyz(void);

/*************************************************************************************************************
 *
 * Name :		  	void convert_xyz_to_roll_pitch()
 *
 *
 *
 * Description :	Funtion to calculate Roll and Pitch values based on the X,Y and Z values
 *
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void convert_xyz_to_roll_pitch(void);

extern float roll, pitch;
extern int16_t acc_X, acc_Y, acc_Z;

#endif
