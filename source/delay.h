/*************************
 *
 *
 *
 *    File name   : delay.h
 *    Description : This file defines the functions which are mentioned in delay.c
 *
 *    Author: TAHER S UJJAINWALA
 * 	  Tools : MCUXpressor IDE
 *    References:
 *    Date  : 09/26/2021
 *
 *
 */
#ifndef DELAY_H_
#define DELAY_H_

/*************************************************************************************************************
 *
 * Name :		  	void delay()
 *
 *
 *
 * Description :	This function hard codes the loop to generate approximate delay using (No Operation) NOP instruction
 *
 *
 * Inputs: Counter
 *
 * Return: NONE
 *************************************************************************************************************/
void delay(int count);

#endif /* DELAY_H_ */
