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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "MKL25Z4.h"
#include "adc.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

//Defining ADC Pin
#define ADC_POS					(20)

uint16_t raw_adc;
int cal_res;


//Function to Initialize the ADC0
void Init_ADC(void) {

	int cal_res;

	//Enabling the ADC0 clock and Port B clock
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

    //Defining the MUX pin as analog
    PORTB->PCR[ADC_POS] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[ADC_POS] |= PORT_PCR_MUX(0);

    //Calibrating the ADC
    do
    {
    	cal_res = adc_cal();
    } while (cal_res == -1);


    // Low power configuration, long sample time, 16 bit single-ended conversion, bus clock input
    ADC0->CFG1 = ADC_CFG1_ADLPC_MASK | ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(3) | ADC_CFG1_ADICLK(0);

    // Software trigger, compare function disabled, DMA disabled, voltage references VREFH and VREFL
    ADC0->SC2 = ADC_SC2_REFSEL(0);


}

//This function is used to read the temperature as mentioned by Dean -
float Read_temp(void){
    float n, temp;

    //Start conversion on channel 26
    ADC0->SC1[0] = 26;

    //Wait for conversion to finish
    while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
        ;
    //Read result, convert to floating-point
    n = (float) ADC0->R[0];

    // Calculate temperature (Celsius) using polynomial equation
    // Assumes ADC is in 16-bit mode, has VRef = 3.3 V
    temp = -36.9861 + n*(0.0155762 + n*(-1.43216E-06 + n*(7.18641E-11
        + n*(-1.84630E-15 + n*(2.32656E-20 + n*(-1.13090E-25))))));
    return temp;
}

//This function is used to read the Raw ADC value which is then converted to get the Internal temperature
uint16_t adc_read() {

	//Start conversin on channel 26
	ADC0->SC1[0] = 26;

	//Wait for conversion to finish
	while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
		;

	//Read the result return the Raw ADC value
	uint16_t raw_temp_adc = ADC0->R[0];

	return raw_temp_adc;
}


//This function is used for calibration of ADC - High and Low calibration - Reference taken from reference manual
int adc_cal(void)
{
	uint16_t calib;

	//16 bits mode, Input Bus Clock divided by 2 (48Mhz / 2), Clock divide by 8 (3 MHz)
	ADC0->CFG1 |= (ADC_CFG1_MODE(3)  | ADC_CFG1_ADICLK(1)| ADC_CFG1_ADIV(3));

	//Enable HW average, Set HW average of 32 samples and Start calibration process
	ADC0->SC3  |= (ADC_SC3_AVGE_MASK |	 ADC_SC3_AVGS(3)   | ADC_SC3_CAL_MASK);

	//Wait for calibration to end
	while (ADC0->SC3 & ADC_SC3_CAL_MASK);

	//Check for successful calibration
	if (ADC0->SC3 & ADC_SC3_CALF_MASK)
		return -1;

	//Calculating the Positive and Negative calibration values
	calib = 0;
	calib += ADC0->CLPS + ADC0->CLP4 + ADC0->CLP3 +
			     ADC0->CLP2 + ADC0->CLP1 + ADC0->CLP0;
	calib /= 2;

	//Set the MSB
	calib |= 0x8000;
	ADC0->PG = calib;

	calib = 0;
	calib += ADC0->CLMS + ADC0->CLM4 + ADC0->CLM3 +
			     ADC0->CLM2 + ADC0->CLM1 + ADC0->CLM0;
	calib /= 2;

	//Set the MSB
	calib |= 0x8000;
	ADC0->MG = calib;

	return 0;
}

