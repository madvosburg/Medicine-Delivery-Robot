/*
*
*   uart_extra_help.c
* Description: This is file is meant for those that would like a little
*              extra help with formatting their code, and followig the Datasheet.
*/

#include "uart_extra_help.h"
#include "timer.h"
//#define REPLACE_ME 0x00
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"


void uart_init(int baud)
{
    SYSCTL_RCGCGPIO_R |= 0x02;      // enable clock GPIOB (page 340)
    SYSCTL_RCGCUART_R |= 0x02;      // enable clock UART1 (page 344)
    GPIO_PORTB_AFSEL_R |= 0b00000011;      // sets PB0 and PB1 as peripherals (page 671)
    GPIO_PORTB_PCTL_R  |= 0x00000011;       // pmc0 and pmc1       (page 688)  also refer to page 650
    GPIO_PORTB_DEN_R   |= 0b00000011;        // enables pb0 and pb1
    GPIO_PORTB_DIR_R  = 0b00000010;         // sets pb0 as output, pb1 as input

 //   GPIO_PORTF_DEN_R = 0b00001111;
 //   GPIO_PORTF_DIR_R = 0xF;

    //compute baud values [UART clock= 16 MHz] 
                                                                       //brd = 16000000 / (16 * 115200) = 8.68056      brdi = 8, fbrd = .6805555556*64 + 0.5 = 44.0555556 = 44

 //   fbrd = REPLACE_ME;

    UART1_CTL_R &= 0xFFFFFFFE;      // disable UART1 (page 918)
    UART1_IBRD_R = 0x8;        // write integer portion of BRD to IBRD
    UART1_FBRD_R = 0x2C;   // write fractional portion of BRD to FBRD
    UART1_LCRH_R = 0x60;        // write serial communication parameters (page 916) * 8bit and no parity
    UART1_CC_R   = 0x0;          // use system clock as clock source (page 939)
    UART1_CTL_R |= 0x1;        // enable UART1

}

void uart_sendChar(char data)
{
   while((UART1_FR_R & 0x20)){} //halts until flag register declares data register is full of outgoing data
   UART1_DR_R = data; //feeds desired data to data register

}

char uart_receive(void)
{
    while((UART1_FR_R & 0x10)){} //halts until flag register declares data register is full of incoming data
    char output = (char)(UART1_DR_R & 0xFF); //receives char from data register

    return output;

}

void uart_sendStr(const char *data) //loops over sendChar for each char of string
{
    int i = 0;
    int len = strlen(data);
    for(i=0; i < len; i++){
        uart_sendChar(data[i]);
    }
	
}

// _PART3


void uart_interrupt_init()
{
    // Enable interrupts for receiving bytes through UART1
    UART1_IM_R |= 0b10000; //enable interrupt on receive - page 924

    // Find the NVIC enable register and bit responsible for UART1 in table 2-9
    // Note: NVIC register descriptions are found in chapter 3.4
    NVIC_EN0_R |= 0b1000000; //enable uart1 interrupts - page 104

    // Find the vector number of UART1 in table 2-9 ! UART1 is 22 from vector number page 104
    IntRegister(INT_UART1, uart_interrupt_handler); //give the microcontroller the address of our interrupt handler - page 104 22 is the vector number

}

void uart_interrupt_handler()
{
// STEP1: Check the Masked Interrupt Status
    if(UART1_MIS_R &= 0b10000){
        uart_data = uart_receive();
        flag = 1;
    }
//STEP2:  Copy the data 

//STEP3:  Clear the interrup   
    GPIO_PORTB_ICR_R |= 0b11111111;
}
