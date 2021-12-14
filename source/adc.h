/*
 * adc.h
 *
 *  Created on: Dec 12, 2021
 *      Author: ujjai
 */

#ifndef ADC_H_
#define ADC_H_

void Init_ADC(void);

int adc_cal(void);

uint16_t adc_read();

#endif /* ADC_H_ */
