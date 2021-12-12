/*************************
 *
 *
 *
 *    File name   : led.c
 *    Description : This file is responsible for all LED control on Start up and Infinite Loop execution
 *    Finite state machine algorithm is used to manage the continuous polling of slider event and based on that led blinking is controlled
 *    The delay is an merely approximation of the actual values that is 100ms, 500ms, 1000ms, 2000ms and 3000ms
 *    The delay function is a counter loop which runs giving appropriate delay of value as passed
 *
 *
 *    Author: TAHER S UJJAINWALA
 * 	  Tools : MCUXpressor IDE
 *    References: Dean Chapter 2 and 3 - INPUT, OUTPUT, FSM
 *    Class notes and slides - Howdy Pierce
 *	  IDE setup and board running help - Nimish Bhide and Mukta Darekar (SA's)
 *    Help for Finite State Machine -  Gaurang Rane
 *    Date  : 09/26/2021
 *
 *
 */
//Including the Header files required for operation
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
//Defining user developed header files
#include "gpio.h"
#include "delay.h"


//Global declaration of LED pins on board
#define RED_LED_PIN (18) //PTB18
#define GREEN_LED_PIN (19) //PTB19
#define BLUE_LED_PIN (1) //PTD1
#define REDE		 (4) //PTD4


void PORTB_CLK(){
	//Enabling Clock for PORT B - LED PIN 18 and 19
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
}

void PORTD_CLK(){
	//Enabling Clock for PORT D - LED PIN 1
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
}

void INIT_LED(){

	//Initializing all LED (RED, GREEN and BLUE) in this function

	//Enabling MUX Port for RED LED
	PORTB->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_PIN] |= PORT_PCR_MUX(1);

	//Enabling MUX Port for GREEN LED
	PORTB->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(1);

	//Enabling MUX Port for BLUE LED
	PORTD->PCR[BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_PIN] |= PORT_PCR_MUX(1);

	//Defining the Data direction register for RED LED
	GPIOB->PDDR |= (1 << RED_LED_PIN);

	//Defining the Data direction register for GREEN LED
	GPIOB->PDDR |= (1 << GREEN_LED_PIN);

	//Defining the Data direction register for BLUE LED
	GPIOD->PDDR |= (1 << BLUE_LED_PIN);
}

void INIT_RW_PIN(){

	//Enabling MUX Port for BLUE LED
	PORTD->PCR[REDE] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[REDE] |= PORT_PCR_MUX(1);

	//Defining the Data direction register for RED LED
	GPIOD->PDDR |= (1 << REDE);
}

//Function to Turn RED LED on - Works on Active LOW Logic - Hence clearing the bit will Turn the LED on
void RED_ON(){
	GPIOB->PCOR |= (1 << RED_LED_PIN);
}

//Function to Turn RED LED off - Works on Active LOW Logic - Hence setting the bit will Turn the LED off
void RED_OFF(){
	GPIOB->PSOR |= (1 << RED_LED_PIN);
}

//Function to Turn GREEN LED on - Works on Active LOW Logic - Hence clearing the bit will Turn the LED on
void GREEN_ON(){
	GPIOB->PCOR |= (1 << GREEN_LED_PIN);
}

//Function to Turn GREEN LED off - Works on Active LOW Logic - Hence setting the bit will Turn the LED off
void GREEN_OFF(){
	GPIOB->PSOR |= (1 << GREEN_LED_PIN);
}

//Function to Turn BLUE LED on - Works on Active LOW Logic - Hence clearing the bit will Turn the LED on
void BLUE_ON(){
	GPIOD->PCOR |= (1 << BLUE_LED_PIN);
}

//Function to Turn BLUE LED off - Works on Active LOW Logic - Hence setting the bit will Turn the LED off
void BLUE_OFF(){
	GPIOD->PSOR |= (1 << BLUE_LED_PIN);
}


//Function to turn all LED Turn off - Default Function
void DEFAULT_LED_STATE(){
	RED_OFF();
	GREEN_OFF();
	BLUE_OFF();

}

//ON -- HIGH
void REDE_ON(){
	GPIOD->PSOR |= (1 << REDE);
}

//OFF -- LOW
void REDE_OFF(){
	GPIOD->PCOR |= (1 << REDE);
}

//Running this function on start-up as mentioned in STEP 1
void INIT_TEST(){

	//Making all the state to OFF
	DEFAULT_LED_STATE();
	//RED Led pattern 500ms ON and 100ms OFF
	RED_ON();
    delay(100);		//Delay for 500ms
    RED_OFF();
    delay(50);		//Delay for 100ms

    //GREEN Led pattern 500ms ON and 100ms OFF
    GREEN_ON();
    delay(100);		//Delay for 500ms
    GREEN_OFF();
    delay(50);		//Delay for 100ms

    //BLUE Led pattern 500ms ON and 100ms OFF
    BLUE_ON();
    delay(100);		//Delay for 500ms
    BLUE_OFF();
    delay(50);		//Delay for 100ms

}

