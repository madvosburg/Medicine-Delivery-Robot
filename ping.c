#include "ping.h"

void ping_init(void){

        //clock to timer 3
        SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;

        //port b clock
        SYSCTL_RCGCGPIO_R |= 0x02;

        //pin 3
        GPIO_PORTB_DEN_R |= 0x08;
        GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB3_T3CCP1;

        //input capture
        TIMER3_CTL_R &= ~TIMER_CTL_TBEN;

        //edge select
        TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;

        //16 bit counter
        TIMER3_CFG_R |= TIMER_CFG_16_BIT;

        //capture mode
        TIMER3_TBMR_R |= TIMER_TBMR_TBMR_CAP;

        //edge-time mode
        TIMER3_TBMR_R |= TIMER_TBMR_TBCMR;

        //count up
        TIMER3_TBMR_R |= TIMER_TBMR_TBCDIR;

        //set upper bound
        TIMER3_TBILR_R |= 0xFFFF;

        //enable capture input
        TIMER3_IMR_R |= TIMER_IMR_CBEIM;

        //clears interrupt
        TIMER3_ICR_R |= TIMER_ICR_CBECINT;

        NVIC_EN1_R |= 0x10;

        IntRegister(INT_TIMER3B, TIMER3B_Handler);

        IntMasterEnable();
}

void send_pulse(){

    //GPIO function
    GPIO_PORTB_AFSEL_R &= ~(0x08);

    //off
    GPIO_PORTB_PCTL_R &= ~(0x0000F000);

    //PB3 output
    GPIO_PORTB_DIR_R |= 0x08;

    //PB3 high
    GPIO_PORTB_DATA_R |= 0x08;

    //wait
    timer_waitMicros(5);

    //PB3 low
    GPIO_PORTB_DATA_R &= 0xF7;
    state = LOW;

    //PB3 input
    GPIO_PORTB_DIR_R &= 0xF7;

    //alt function
    GPIO_PORTB_AFSEL_R |= 0x08;

    //T3CCP1
    GPIO_PORTB_PCTL_R |= 0x00007000;

    //enable TIMER3B and count
    TIMER3_CTL_R |= TIMER_CTL_TBEN;

}

void TIMER3B_Handler(void){
        if(state == LOW){
            state = HIGH;
            rising_time = TIMER3_TBR_R;
        }else if(state == HIGH){
            falling_time =  TIMER3_TBR_R;
            state = DONE;
        }
        TIMER3_ICR_R = TIMER_ICR_CBECINT;
    }

int ping_read(void){
    send_pulse();
    while(state != DONE){} //waits until done
    return(17 * (falling_time - rising_time) / 16000);

}


