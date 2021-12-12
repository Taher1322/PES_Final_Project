/*****************************************************************************
* Copyright (C) 2021 by Taher Ujjainwala
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Taher Ujjainwala and the University of Colorado are not liable for
* any misuse of this material.
*
*****************************************************************************/
/**
* @file led.h
* @brief An abstraction for function definitions
*
* This header file provides an abstraction of
* changing the Led status via function calls.
*
* @author Taher Ujjainwala
* @date September 29 2021
* @version 1.0
*
*/

/*************************
 *
 *
 *
 *    File name   : led.h
 *    Description : This file is responsible for all functions which are implemented in led.c for program execution

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
#ifndef LED_H_
#define LED_H_

/*************************************************************************************************************
 *
 * Name :		  	void PORTB_CLK()
 *
 *
 *
 * Description :	This function initializes clock for PORTB
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void PORTB_CLK();

/*************************************************************************************************************
 *
 * Name :		  	void PORTD_CLK()
 *
 *
 *
 * Description :	This function initializes clock for PORTD
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/

void PORTD_CLK();

/*************************************************************************************************************
 *
 * Name :		  	void INIT_LED()
 *
 *
 *
 * Description :	This function enables the MUX port, Data Direction register for RED, GREEN and BLUE onboard LED
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void INIT_LED();

/*************************************************************************************************************
 *
 * Name :		  	void DEFAULT_LED_STATE()
 *
 *
 *
 * Description :	This function initializes RED, GREEN and BLUE led as LOW
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void DEFAULT_LED_STATE();


/*************************************************************************************************************
 *
 * Name :		  	void INIT_TEST()
 *
 *
 *
 * Description :	This function initializes RED, GREEN and BLUE led as follows
 * RED ON for 500ms and OFF for 100ms
 * GREEN ON for 500ms and OFF for 100ms
 * BLUE ON for 500ms and OFF for 100ms
 * WHITE ON for 100ms and OFF for 100ms
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void INIT_TEST();

void INIT_RW_PIN();

void INIT_TEST();

/*************************************************************************************************************
 *
 * Name :		  	void WHITE_PAT()
 *
 *
 *
 * Description :	This function initializes WHITE led as follows
 * WHITE ON for 500ms and OFF for 500ms
 * WHITE ON for 1000ms and OFF for 500ms
 * WHITE ON for 2000ms and OFF for 500ms
 * WHITE ON for 3000ms and OFF for 500ms
 *
 *This function is developed for internal testing and not used in program execution
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void WHITE_PAT();

/*************************************************************************************************************
 *
 * Name :		  	void RED_ON()
 *
 *
 *
 * Description :	This function initializes RED led as ON
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void RED_ON();


/*************************************************************************************************************
 *
 * Name :		  	void RED_OFF()
 *
 *
 *
 * Description :	This function initializes RED led as OFF
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void RED_OFF();


/*************************************************************************************************************
 *
 * Name :		  	void GREEN_ON()
 *
 *
 *
 * Description :	This function initializes GREEN led as ON
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void GREEN_ON();

/*************************************************************************************************************
 *
 * Name :		  	void GREEN_OFF()
 *
 *
 *
 * Description :	This function initializes GREEN led as OFF
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void GREEN_OFF();

/*************************************************************************************************************
 *
 * Name :		  	void BLUE_ON()
 *
 *
 *
 * Description :	This function initializes BLUE led as ON
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void BLUE_ON();

/*************************************************************************************************************
 *
 * Name :		  	void BLUE_OFF()
 *
 *
 *
 * Description :	This function initializes BLUE led as OFF
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void BLUE_OFF();

void REDE_ON();

void REDE_OFF();

#endif /* LED_H_ */
