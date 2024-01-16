

#include "adc.h"

void adc_init(){

      //enable port B
      SYSCTL_RCGCGPIO_R |= 0b000010;

      //enable clock for ADC0
      SYSCTL_RCGCADC_R |= 0x1;

      //enable port B pin 4 to work as alternate functions
      GPIO_PORTB_AFSEL_R |= 0x10;

      //set pin to input
      GPIO_PORTB_DEN_R &= 0b11101111;

      //enable GPIO port b direction
      GPIO_PORTB_DIR_R &= 0b11101111;

      //disable analog isolation for the pin
      GPIO_PORTB_AMSEL_R |= 0x10;

      //initialize the port trigger source as processor (default)
      GPIO_PORTB_ADCCTL_R = 0x00;

      //disable SS0 sample sequencer to configure it
      ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0;

      //set 1st sample to use the AIN10 ADC pin
      ADC0_SSMUX0_R |= 0x000A;

      //enable raw interrupt status     //0x6
      ADC0_SSCTL0_R |= (ADC_SSCTL0_IE0 | ADC_SSCTL0_END0);

      //enable oversampling to average
      ADC0_SAC_R |= ADC_SAC_AVG_16X;                //average mechanism

      //re-enable ADC0 SS0
      ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;

      //clear interrupt flags
      ADC0_ISC_R |= ADC_ISC_IN0;

}

int adc_read(){

    //start conversion
    ADC0_PSSI_R = ADC_PSSI_SS0;

    //wait for conversions to complete
	while((ADC0_RIS_R & ADC_RIS_INR0) == 0){}

	//clear interrupt
	ADC0_ISC_R = ADC_ISC_IN0;

	//return result
    return (int)ADC0_SSFIFO0_R;

}



/**
    SYSCTL_RCGCGPIO_R |= 0b00010;
    GPIO_PORTB_AFSEL_R |= 0x10;
    GPIO_PORTB_DIR_R &= 0b11101111;
    GPIO_PORTB_DEN_R &= 0b11101111;
    GPIO_PORTB_AMSEL_R |= 0x10;
    GPIO_PORTB_ADCCTL_R = 0x00;

    SYSCTL_RCGCADC_R |= 0x1;

  //  ADC0_ACTSS_R |= 0b1000;
      ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0;   //disable to configure
      ADC0_EMUX_R = ADC_EMUX_EM0_PROCESSOR;
      ADC0_SSMUX0_R |= 0x000A;  //AIN10
      ADC0_SSCTL0_R |= (ADC_SSCTL0_IE0 | ADC_SSCTL0_END0);  //enable raw interrupt
      ADC0_SAC_R |= ADC_SAC_AVG_64X;
      ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;  //re-enable
**/
    // ADC0_SSMUX3_R =

//initialize the ADC trigger source as processor (default)
    // ADC0_EMUX_R = ADC_EMUX_EM0_PROCESSOR;
