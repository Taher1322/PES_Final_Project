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
 *    File name   : i2c.c
 *    Description : This file defines the function to initialize I2C0 bus protocol
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

//Including all header files
#include <stdint.h>

//Defining Macros for all I2C bus protocol Initialization and Run
#define I2C_M_START 	I2C0->C1 |= I2C_C1_MST_MASK
#define I2C_M_STOP  	I2C0->C1 &= ~I2C_C1_MST_MASK
#define I2C_M_RSTART 	I2C0->C1 |= I2C_C1_RSTA_MASK

//Macros for Transmit and Receive
#define I2C_TRAN			I2C0->C1 |= I2C_C1_TX_MASK
#define I2C_REC				I2C0->C1 &= ~I2C_C1_TX_MASK

//Macros for Acknowledgemen and Waiting
#define BUSY_ACK 	    while(I2C0->S & 0x01)
#define TRANS_COMP		while(!(I2C0->S & 0x80))
#define I2C_WAIT 			i2c_wait();

//Macros for Acknowledge
#define NACK 	        I2C0->C1 |= I2C_C1_TXAK_MASK
#define ACK           I2C0->C1 &= ~I2C_C1_TXAK_MASK

/*************************************************************************************************************
 *
 * Name :		  	void Init_I2C(void)
 *
 *
 *
 * Description :	Function to Initialize I2C bus
 *
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void Init_I2C(void);

/*************************************************************************************************************
 *
 * Name :		  	void i2c_start(void)
 *
 *
 *
 * Description :	This function will send start sequence
 *
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void i2c_start(void);

/*************************************************************************************************************
 *
 * Name :		  	void i2c_read_setup(uint8_t dev, uint8_t address)
 *
 *
 *
 * Description :	This function will send device and register addresses
 *
 *
 * Inputs: Device and Address
 *
 * Return: NONE
 *************************************************************************************************************/
void i2c_read_setup(uint8_t dev, uint8_t address);

/*************************************************************************************************************
 *
 * Name :		  	uint8_t i2c_repeated_read(uint8_t)
 *
 *
 *
 * Description :	This function read a byte and gets the ack/nack
 *
 *
 * Inputs: Last Read Input
 *
 * Return: Data
 *************************************************************************************************************/
uint8_t i2c_repeated_read(uint8_t);

/*************************************************************************************************************
 *
 * Name :		  	uint8_t i2c_read_byte(uint8_t dev, uint8_t address)
 *
 *
 *
 * Description :	This function reads and write a single byte, it uses 7 bit bus addressing to read from device address
 *
 *
 * Inputs: Device and Address
 *
 * Return: Data
 *************************************************************************************************************/
uint8_t i2c_read_byte(uint8_t dev, uint8_t address);

/*************************************************************************************************************
 *
 * Name :		  	void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data)
 *
 *
 *
 * Description :	This function writes a byte to device address specified and uses 7 bit addressing
 *
 *
 * Inputs: Device, Address and Data
 *
 * Return: NONE
 *************************************************************************************************************/
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data);
