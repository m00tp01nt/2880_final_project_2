/*
 * adc.h
 *
 *  Created on: Mar 25, 2025
 *      Author: esliandy
 */
#ifndef ADC_H_

#define ADC_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>

void adc_init ();
uint16_t adc_read ();

#endif
