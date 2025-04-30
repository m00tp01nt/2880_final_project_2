/*
 * adc.c
 *  LAB 08
 *  Created on: Mar 25, 2025
 *      Author: esliandy
 */
#include "adc.h"

void adc_init () {

    SYSCTL_RCGCADC_R |= 0b11;
    SYSCTL_RCGCGPIO_R |= 0b10;

    while (!(SYSCTL_PRGPIO_R & 0b10)) {};

    GPIO_PORTB_DIR_R &= 0x10;       // Digital Enable
    GPIO_PORTB_AFSEL_R |= 0x10;     // Alternate Function
    GPIO_PORTB_DEN_R &= ~0x10;      // Make PB4 input
    GPIO_PORTB_AMSEL_R |= 0x10;     // Enable analog func for PB4

    ADC0_PC_R &= ~0xF;
    ADC0_PC_R |= 0x1;
    ADC0_SSPRI_R |= 0x3000; //sample sequencer 3
    ADC0_ACTSS_R &= ~0x0008; //Disable SS3 to configure
    ADC0_EMUX_R &= ~0xF000; //Set sequencer 3 as event trigger
    ADC0_SSMUX3_R &= ~0x000F;
    ADC0_SSMUX3_R += 10; //Channel 10
    ADC0_SSCTL3_R = 0x0006;
    ADC0_IM_R &= 0x0008; //Disable interrupt mask for SS3
    ADC0_ACTSS_R |= 0x0008; //Enable SS3
}

uint16_t adc_read () {
    int result;

    ADC0_PSSI_R = 0x008; //initiate SS3

    while ((ADC0_RIS_R & 0x08) == 0) {}; //wait for conversion

    result = ADC0_SSFIFO3_R & 0xFFF;
    ADC0_ISC_R = 0x0008; //mark read completion

    return result;
}



