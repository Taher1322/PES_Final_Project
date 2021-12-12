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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"

//Defining the UART parameters and easy to change in future
#define UART_OVERSAMPLE_RATE 	(16)		//Over sampling rate
#define SYS_CLOCK				(24e6)		//Running the system on 24MHz
#define PARITY      			(0)			//Parity None
#define DATA_BITS   			(0)         //2 modes for data - 8 bits and 9 bits --> For 8 bits - 0 and For 9 bits - 1
#define STOP_BITS   			(1)         //2 modes for Stop bit --> For 1 stop bit - 0 and For 2 Stop bits - 1

// UART1 uses the bus clock
#define BUSCLOCK_FREQUENCY (24000000U)

volatile extern char input_key;

//void transmit_data(char *pdata);

//Function Initializing UART
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
/*
	NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);
*/
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

	//Clear error flags
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



void uart_init_howdy(uint32_t baud_rate)
{
	uint16_t sbr;

  //gUART_Tx_Q = xQueueCreate(UART_TX_Q_SIZE, sizeof(uint8_t));
  //gUART_Rx_Q = xQueueCreate(UART_RX_Q_SIZE, sizeof(uint8_t));

  // UART1: Just the defaults, nothing unusual
  SIM->SOPT5 = SIM_SOPT5_UART1ODE(0) | SIM_SOPT5_UART1RXSRC(0) | SIM_SOPT5_UART1TXSRC(0);

	// Enable clock gating for UART1 and PORTE
	SIM->SCGC4 |= SIM_SCGC4_UART1(1);
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	// Set pins to UART1 Rx and Tx
  PORTE->PCR[0] &= ~PORT_PCR_MUX_MASK;  // Tx
  PORTE->PCR[0] |= PORT_PCR_MUX(3);
  PORTE->PCR[1] &= ~PORT_PCR_MUX_MASK;  // Rx
	PORTE->PCR[1] |= PORT_PCR_MUX(3);

	// Disable transmitter and receiver during init
	UART1->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

	// Set baud rate and oversampling ratio
	sbr = (uint16_t)(BUSCLOCK_FREQUENCY/(baud_rate * UART_OVERSAMPLE_RATE));
	UART1->BDH &= ~UART_BDH_SBR_MASK;
	UART1->BDH |= UART_BDH_SBR(sbr>>8);
	UART1->BDL = UART_BDL_SBR(sbr);
	UART1->C4 = 0;

	// Disable interrupts for RX active edge and LIN break detect, select one stop bits
  UART1->BDH |= UART_BDH_RXEDGIE(0) | UART_BDH_LBKDIE(0) | UART_BDH_SBNS(0);

	// Configure serial port as 8 bits, no parity
	UART1->C1 = UART_C1_LOOPS(0) | UART_C1_M(0) | UART_C1_PE(0) | UART_C1_PT(0);
	// Don't invert transmit data, don't enable interrupts for errors
  UART1->C3 = UART_C3_TXINV(0) | UART_C3_ORIE(1)| UART_C3_NEIE(1)
    | UART_C3_FEIE(1) | UART_C3_PEIE(1);

	// Do not invert received data
  UART1->S2 = UART_S2_RXINV(0);


	NVIC_SetPriority(UART1_IRQn, 3); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART1_IRQn);
	NVIC_EnableIRQ(UART1_IRQn);

	// Enable receive interrupts; transmit interrupts enabled when data
	// is waiting to be transmitted
	UART1->C2 |= UART_C2_RIE(1);

	// Enable UART receiver and transmitter
	UART1->C2 |= UART_C2_RE(1) | UART_C2_TE(1);
}


void transmit_data0(unsigned char *pdata, int total_character)
{
	// Wait until complete string is transmitted on serial port
	// and every byte is shifted out of Transmit buffer before
	// loading new byte
	uint8_t count = total_character - 1;

	while(count--)
	//while (*pdata)
	{
		//__disable_irq();
		UART0->D = *pdata;

		// Wait until byte is transmitted from Tx Buffer
		while (!(UART0->S1 & UART_S1_TDRE_MASK)) {
		}

		//__enable_irq();

		pdata++;
	}
}

void transmit_data1(unsigned char *pdata, int total_character)
{
	// Wait until complete string is transmitted on serial port
	// and every byte is shifted out of Transmit buffer before
	// loading new byte
	uint8_t count = total_character - 1;

	while(count--)
	//while (*pdata)
	{
		//__disable_irq();
		UART1->D = *pdata;

		// Wait until byte is transmitted from Tx Buffer
		while (!(UART1->S1 & UART_S1_TDRE_MASK)) {
		}

		//__enable_irq();

		pdata++;
	}
}

void UART0_IRQHandler(void) {
	uint8_t ch;

	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK |
		UART_S1_FE_MASK | UART_S1_PF_MASK)) {
			// clear the error flags
			UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
									UART0_S1_FE_MASK | UART0_S1_PF_MASK;
			// read the data register to clear RDRF
			ch = UART0->D;

	}
}

uint8_t UART0_Receive_Poll(void) {
		int i=0;
		char rgucTemp[100] = "\0";

		memset(rgucTemp, 0, sizeof(rgucTemp));

		while (!(UART0->S1 & UART0_S1_RDRF_MASK))
		;
			//{
			rgucTemp[i] = UART0->D;
			i++;
		//}

		putchar(rgucTemp[0]);
		putchar(rgucTemp[1]);

		return 0;
		//return UART0->D;
		//putchar(rgucTemp);
}

void UART1_IRQHandler(void){
//uint8_t UART1_Receive_Poll(void) {
		while (!(UART1->S1 & UART_S1_RDRF_MASK))
			;
		input_key = UART1->D;
		//return UART1->D;
}

