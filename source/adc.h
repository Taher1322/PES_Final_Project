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
 *    File name   : adc.c
 *    Description : This file defines the function to implement ADC to get Internal Temperature sensor value
 *
 *    Author TAHER UJJAINWALA
 * 	  Tools : MCUXpressor IDE
 * 	  General References used in development:
 * 	  DEAN ADC section and Core part of reference manual
 * 	  AN3031 Datasheet to get internal temperature value
 *
 *
 *    Date  : 12/13/2021
 *
 *
 */

//Including all the header files
#ifndef ADC_H_
#define ADC_H_


/*************************************************************************************************************
 *
 * Name :		  	void Init_ADC()
 *
 *
 *
 * Description :	Function to Initialize the ADC0
 *
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
void Init_ADC(void);

/*************************************************************************************************************
 *
 * Name :		  	int adc_cal()
 *
 *
 *
 * Description :	This function is used for calibration of ADC - High and Low calibration - Reference taken from reference manual
 *
 *
 * Inputs: NONE
 *
 * Return: NONE
 *************************************************************************************************************/
int adc_cal(void);

/*************************************************************************************************************
 *
 * Name :		  	uint16_t adc_read()
 *
 *
 *
 * Description :	This function is used to read the Raw ADC value which is then converted to get the Internal temperature
 *
 *
 * Inputs: NONE
 *
 * Return: Raw ADC value
 *************************************************************************************************************/
uint16_t adc_read();

#endif /* ADC_H_ */
