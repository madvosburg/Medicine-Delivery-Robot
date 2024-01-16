#include <math.h>
#include "servo.h"
#include "Timer.h"

// 16MHz crystal -> 62.5ns/cycle -> 320000 clock cycles for a period of 20ms

extern volatile float match_offset = -3.7;
extern volatile float match_multiplier = 1.76;
extern volatile int prev_deg = 0;

void servo_init(void){ //sets registers to begin PWM function
    SYSCTL_RCGCGPIO_R |= 0b10; //enables port B
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1; //enable clock to timer 1
    GPIO_PORTB_DEN_R |= 0b100000; //enables PB5
    GPIO_PORTB_AFSEL_R |= 0b100000; //sets PB5 AFSEL
    GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB5_T1CCP1; //sets PB5 PCTL to T1CCP1 (timer)

    TIMER1_CTL_R &= ~(0x0100); //timer B disable for setup
    TIMER1_CFG_R = 0x4; //16-bit config
    TIMER1_TBMR_R |= 0b1010; //PWM enabled, edge-count mode, periodic timer mode

}

int servo_move(float degrees, int holdFlag){
    float duty_cycle = (5 + match_offset + (degrees/36.0)) / 100.0; //duty cycle needed for given angle; send to timer match register after processing; ranges from 5%->10% (as 0.05->0.1)
    int match = 0x4E200 - (int)((duty_cycle * 0x4E200 * match_multiplier)); //match value is start_val - (start_val * duty_cycle). also factors in the offset


    TIMER1_TBPR_R = 0x4; //prescale offset for 20ms period
    TIMER1_TBILR_R = 0xE200; //sets initial timer value
    TIMER1_TBPMR_R = 0x4; //sets match prescale-- always 4
    TIMER1_TBMATCHR_R &= 0x00000; //clear match value
    TIMER1_TBMATCHR_R |= match; //makes match value follow duty-cycle
    TIMER1_CTL_R |= 0x0100; //timer ON

    timer_waitMillis(50 + ((abs(degrees - prev_deg)) * 10));
    //timer_waitMillis(((1000 / 260) * (abs(degrees - prev_deg) + 50) * 2)); // ~260 deg/sec movement speed of servo @ 5V; this time is that plus a bit of padding & spacing

    //timer turns off only if holdFlag is set to 0
    if(!holdFlag){
        TIMER1_CTL_R &= ~(0x0100);  //timer OFF
    }

    prev_deg = degrees;

    return(1);
}

