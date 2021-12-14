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

//Including all the header files
#include <MKL25Z4.H>
#include "i2c.h"

//Defining variables
int lock_detect=0;
int i2c_lock=0;


//Function to Initialize I2C bus
void Init_I2C(void)
{
	//Enabling I2C0 clock and Port E clock
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK);

	//Defining the MUX port as Alternative Function on Port E 24 and 25
	PORTE->PCR[24] |= PORT_PCR_MUX(5);
	PORTE->PCR[25] |= PORT_PCR_MUX(5);

	//Setting the Baud Rate to 100K using the formula Baud = Bus Freq/ (SCL_DIV + MUL)
 	//~400k = 24M/(64); icr=0x12 sets scl_div to 64
	I2C0->F = (I2C_F_ICR(0x10) | I2C_F_MULT(0));

	//Enabling I2C and Set to Master Mode
	I2C0->C1 |= (I2C_C1_IICEN_MASK);

	//Select High Drive Mode
	I2C0->C2 |= (I2C_C2_HDRS_MASK);
}

//Function to Check the I2C bus busy status
void i2c_busy(void){

	//Start Signal
	lock_detect=0;
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C_TRAN;
	I2C_M_START;
	I2C0->C1 |=  I2C_C1_IICEN_MASK;

	//Write to clear line
	//Select the Master Mode and Set Transmit mode
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK;
	I2C0->D = 0xFF;

	//Hard spin loop waiting for the Interrupt
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0U) {
	}
	//Clear the Interrupt Bit
	I2C0->S |= I2C_S_IICIF_MASK;


	//Clear the arbitration error flag
	I2C0->S |= I2C_S_ARBL_MASK;


	//Send start, set transmit mode, start signal generate
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;

	//Wait until Start is send
	I2C0->C1 |= I2C_C1_IICEN_MASK;


	//Send stop once the start is sent
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;

	//Set Slave mode
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->C1 &= ~I2C_C1_MST_MASK;

	//Set the receiver mask
	I2C0->C1 &= ~I2C_C1_TX_MASK;
	I2C0->C1 |= I2C_C1_IICEN_MASK;



	//Wait
	I2C0->S |= I2C_S_IICIF_MASK;

	//Clear the arbitration error flag and interrupt flag
	I2C0->S |= I2C_S_ARBL_MASK;
	lock_detect=0;
	i2c_lock=1;
}

//This function will Wait for the System
void i2c_wait(void) {
	lock_detect = 0;
	while(((I2C0->S & I2C_S_IICIF_MASK)==0) & (lock_detect < 200)) {
		lock_detect++;
	}
	if (lock_detect >= 200)
		i2c_busy();
	I2C0->S |= I2C_S_IICIF_MASK;
}

//This function will send start sequence
void i2c_start()
{
	//Set to Transmit Mode
	I2C_TRAN;

	//Send the Start Signal
	I2C_M_START;
}

//This function will send device and register addresses
void i2c_read_setup(uint8_t dev, uint8_t address)
{
	//Sending the device address and waiting for completion
	I2C0->D = dev;
	I2C_WAIT

	//Sending the read address and waiting for completion
	I2C0->D =  address;
	I2C_WAIT

	//Repeating the start by sending the device address
	I2C_M_RSTART;
	I2C0->D = (dev|0x1);
	//Waiting for completion
	I2C_WAIT

	//Set to receive mode
	I2C_REC;

}

//This function read a byte and gets the ack/nack
uint8_t i2c_repeated_read(uint8_t isLastRead)
{
	uint8_t data;

	lock_detect = 0;

	//Setting the NACK and ACK after reading
	if(isLastRead)	{
		NACK;
	} else	{
		ACK;
	}

	//Dummy reading and waiting for completion
	data = I2C0->D;
	I2C_WAIT

	//Sending stop
	if(isLastRead)	{
		I2C_M_STOP;
	}

	//Reading the actual data
	data = I2C0->D;

	return  data;
}



//This function reads and write a single byte, it uses 7 bit bus addressing to read from device address
uint8_t i2c_read_byte(uint8_t dev, uint8_t address)
{
	uint8_t data;

	//Setting to transmit mode, sending start, sending device address and waiting for completion
	I2C_TRAN;
	I2C_M_START;
	I2C0->D = dev;
	I2C_WAIT

	//Sending the read address and waiting for completion
	I2C0->D =  address;
	I2C_WAIT

	//Sending the start again with the device address and waiting for completion
	I2C_M_RSTART;
	I2C0->D = (dev|0x1);
	I2C_WAIT

	//Setting to receive mode and setting NACK
	I2C_REC;
	NACK;

	//Dummy reading the data and waiting for completion
	data = I2C0->D;
	I2C_WAIT

	//Sending the stop and reading the actual data
	I2C_M_STOP;
	data = I2C0->D;

	return data;
}


//This function writes a byte to device address specified and uses 7 bit addressing
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data)
{

	//Setting to transmit mode, sending start, sending device address and waiting for acknowledge
	I2C_TRAN;
	I2C_M_START;
	I2C0->D = dev;
	I2C_WAIT

	//Sending the write address and waiting
	I2C0->D =  address;
	I2C_WAIT

	//Sendind the data to write, waiting and stopping
	I2C0->D = data;
	I2C_WAIT
	I2C_M_STOP;

}
