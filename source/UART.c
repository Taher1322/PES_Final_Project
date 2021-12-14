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
 *    File name   : UART.c
 *    Description : This file defines all the functions to Initialize UART0, Ties the getchar(), sscanf() with sysread(), also ties printf() and putchar() with syswrite()
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

//Including all the header files
#include "MKL25Z4.h"
#include "uart.h"
#include "cbfifo.h"
#include "gpio.h"
#include "delay.h"

//Defining the UART parameters and easy to change in future
#define UART_OVERSAMPLE_RATE 	(16)		//Over sampling rate
#define SYS_CLOCK				(24e6)		//Running the system on 24MHz
#define PARITY      			(0)			//Parity None
#define DATA_BITS   			(0)         //2 modes for data - 8 bits and 9 bits --> For 8 bits - 0 and For 9 bits - 1
#define STOP_BITS   			(1)         //2 modes for Stop bit --> For 1 stop bit - 0 and For 2 Stop bits - 1

//Defining the request frame with 16bit Modbus CRC as per the datasheet - Attached in Github repo in Modbus Memory Mapping
unsigned char Meter_Address[4][10] = {
	{0x00, 0x01, 0x03, 0x00, 0x63, 0x00, 0x02, 0x34, 0x15, '\0'},	 //R Phase Voltage - V
	{0x00, 0x01, 0x03, 0x00, 0x71, 0x00, 0x02, 0x94, 0x10, '\0'}, 	 //R Phase LineCurrent - A
	{0x00, 0x01, 0x03, 0x00, 0x85, 0x00, 0x02, 0xD5, 0xE2, '\0'},	  //Q1 Power factor
	{0x00, 0x01, 0x03, 0x00, 0xDF, 0x00, 0x02, 0xF5, 0xF1, '\0'},	//Forward Active energy - KWh
};

//Variables used in run time
uint8_t input_key;
char rgucTemp[100] = "\0";
int status_flag = 0;

//Function Initializing UART0
void Init_UART0(uint32_t baud_rate) {
	uint16_t sbr;
	//uint8_t temp;

	//Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;


	//Make sure transmitter and receiver are disabled before initializing
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	//Set UART clock to 24 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);//mcg or mcg/2 clock

	//Set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	//Set baud rate and oversampling ratio
	sbr = (uint16_t)((SYS_CLOCK)/(baud_rate * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	//Disable interrupts for RX active edge and LIN break detect, select one stop bit
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(STOP_BITS) | UART0_BDH_LBKDIE(0);

	//Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(DATA_BITS) | UART0_C1_PE(PARITY);

	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	//Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	//Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);


	//Enable interrupts. Listing 8.11 on p. 234

	NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	//Enable receive interrupts but not transmit interrupts yet
	UART0->C2 |= UART_C2_RIE(1);

	//Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

	// Clear the UART RDRF flag
	//temp = UART0->D;
	UART0->S1 &= ~UART0_S1_RDRF_MASK;

	//Was getting warming when removed this lines. Hence to remove the warning this is commented
	//if(temp)
	//{}
}

//UART0 Interrupt handler
void UART0_IRQHandler(void) {

	uint8_t ch;

	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK |
			UART_S1_FE_MASK | UART_S1_PF_MASK))
	{

			//Clear the error flags
			UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
					UART0_S1_FE_MASK | UART0_S1_PF_MASK;
			//Read the data register to clear RDRF
			ch = UART0->D;
	}

	if (UART0->S1 & UART0_S1_RDRF_MASK) 			//Read interrupt
	{
		//Received a character
		ch = UART0->D;
		if(!(cbfifo_capacity(RXD) == cbfifo_length(RXD)))
		{
			cbfifo_enqueue(&ch, 1, RXD);			//Enqueue the character to Receive Buffer
		}
		else
		{
			//Discard the character because the queue is full
		}

	}

	if ( (UART0->C2 & UART0_C2_TIE_MASK) && (UART0->S1 & UART0_S1_TDRE_MASK) )  // tx buffer empty
	{
		// can send another character
		if(cbfifo_length(TXD)!=0) //not empty
		{
			 cbfifo_dequeue(&ch, 1, TXD);			//Dequeue the character from Transmit buffer to UART
			 UART0->D = ch;
		}
		else
			UART0->C2 &= ~UART0_C2_TIE_MASK;		//Queue is empty so disabling the Trasnmit interrupt
	}
}

//sysread function tied to work internally like getchar() and sscanf()
//This function dequeques the character from receiver buffer

int __sys_readc(void)
{
	int ch;

	while(cbfifo_length(RXD)==0)				//Will return -1 if the receive buffer is empty
					;
	if (cbfifo_dequeue(&ch, 1, RXD) == 1)
	{
		return ch;
	}
	else
	{
		return -1;
	}
}



//syswrite function tied to work internally like putchar() and printf()
//This function enqueques the character from transmit buffer
int __sys_write(int handle, char *buf, int size)
{

	while(cbfifo_length(TXD) == cbfifo_capacity(TXD))
		;

	if (cbfifo_enqueue(buf, size, TXD) == 0)				//Will return -1 if buffer is full
	{

		return -1;
	}

	else
	{
		// start transmitter if it isn't already running
		if (!(UART0->C2 & UART0_C2_TIE_MASK)) {
		UART0->C2 |= UART0_C2_TIE(1);
		}
		return  0;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////
//Function to Initialize UART1
void Init_UART1(uint32_t baud_rate) {
	uint16_t sbr;
	//uint8_t temp;

	//Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART1(1);
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;


	//Make sure transmitter and receiver are disabled before initializing
	//UART1->C2 &= ~UART_C2_TE(0) & ~UART_C2_RE(0);


	// Disable transmitter and receiver during init
	UART1->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

	//Set UART clock to 24 MHz clock
	//SIM->SOPT2 |= SIM_SOPT2_UARTSRC(1);//mcg or mcg/2 clock
	SIM->SOPT5 = SIM_SOPT5_UART1ODE(0);
	SIM->SOPT5 |= SIM_SOPT5_UART1RXSRC(0);
	SIM->SOPT5 |= SIM_SOPT5_UART1TXSRC(0);


	//Set pins to UART0 Rx and Tx

	//PORTE->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	//PORTE->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	// Set pins to UART1 Rx and Tx
	PORTE->PCR[0] &= ~PORT_PCR_MUX_MASK;  // Tx
	PORTE->PCR[0] |= PORT_PCR_MUX(3);
	PORTE->PCR[1] &= ~PORT_PCR_MUX_MASK;  // Rx
	PORTE->PCR[1] |= PORT_PCR_MUX(3);


	//Set baud rate and oversampling ratio
	sbr = (uint16_t)((SYS_CLOCK)/(baud_rate * UART_OVERSAMPLE_RATE));
	UART1->BDH &= ~UART_BDH_SBR_MASK;
	UART1->BDH |= UART_BDH_SBR(sbr>>8);
	UART1->BDL = UART_BDL_SBR(sbr);
	UART1->C4 = 0;

	//Disable interrupts for RX active edge and LIN break detect, select one stop bit
	UART1->BDH |= UART_BDH_RXEDGIE(0) | UART_BDH_SBNS(0) | UART_BDH_LBKDIE(0);

	//Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART1->C1 = UART_C1_LOOPS(0) | UART_C1_M(0) | UART_C1_PE(0) | UART_C1_PT(0);

	// Don't invert transmit data, don't enable interrupts for errors
	UART1->C3 = UART_C3_TXINV(0) | UART_C3_ORIE(0)| UART_C3_NEIE(0)
			| UART_C3_FEIE(1) | UART_C3_PEIE(1);

	//Clear error flags - Read only flags
	//UART1->S1 = UART_S1_OR(1) | UART_S1_NF(1) | UART_S1_FE(1) | UART_S1_PF(1);

	//Send LSB first, do not invert received data
	//UART1->S2 = UART_S2_MSBF(0) | UART_S2_RXINV(0);
	UART1->S2 |= UART_S2_RXINV(0);


	//Enable interrupts. Listing 8.11 on p. 234

	NVIC_SetPriority(UART1_IRQn, 3); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART1_IRQn);
	NVIC_EnableIRQ(UART1_IRQn);

	//Enable receive interrupts but not transmit interrupts yet
	UART1->C2 |= UART_C2_RIE(1);



	//Enable UART receiver and transmitter
	UART1->C2 |= UART_C2_RE(1) | UART_C2_TE(1);

	// Clear the UART RDRF flag
	//temp = UART0->D;
	//UART1->S1 &= ~UART1_S1_RDRF(0);

	//Was getting warming when removed this lines. Hence to remove the warning this is commented
	//if(temp)
	//{}
}

//Function to transmit the request frames
void transmit_data1(unsigned char *pdata, int total_character)
{
	// Wait until complete string is transmitted on serial port
	// and every byte is shifted out of Transmit buffer before
	// loading new byte
	uint8_t count = total_character - 1;

	while(count!=0)
	//while (*pdata!= '\0')
	{
		//__disable_irq();
		UART1->D = *pdata;

		// Wait until byte is transmitted from Tx Buffer
		while (!(UART1->S1 & UART_S1_TDRE_MASK)) {
		}

		//__enable_irq();

		pdata++;
		count--;
	}
}

//UART1 receiver handler
void UART1_IRQHandler(void)
{
//uint8_t UART1_Receive_Poll(void) {

	int i=0;
	//Waiting to read the data
	while (!(UART1->S1 & UART_S1_RDRF_MASK))
		;

	//Storing the data in Array
	rgucTemp[i] = UART1->D;
	i++;
	//Defining the status flag to keep a track of actual and no data
	status_flag = 1;
}

//Funtion to read modbus data, deocde and return the values
int modbus_read(int i)
{
	int fReadData = -1;

	memset(rgucTemp, 0, 50);

	//Flow to implement Rs485 protocol - GPIO High to few ms, then transmit the request frame, turn the GPIO Low
	//Then wait for 500ms - Standard as per the meter and get the response in UART1 handler - Which is then decoded and returned
	REDE_ON();
	delay(50);
	transmit_data1(Meter_Address[i], sizeof(Meter_Address[i]));
	delay(100);
	REDE_OFF();
	delay(500);

	//If valid data then decode the data as per hex converter
	if(status_flag == 1)
	{
		*((char *)&fReadData + 3) = rgucTemp[5];
		*((char *)&fReadData + 2) = rgucTemp[6];
		*((char *)&fReadData + 1) = rgucTemp[3];
		*((char *)&fReadData + 0) = rgucTemp[4];
		status_flag = 0;
	}

	return fReadData;

}
