#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "Timer.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "servo.h"
#include "adc.h"
#include "ping.h"
#include "driverlib/interrupt.h"
#include "scan.h"

int IRconversion(double irInput){
    irInput /= 1000;
    irInput = 55.7 * (pow(irInput, -1.69));
    return ((int)irInput);
}

void init_scan(){
    adc_init();
    servo_init();
    ping_init();
}

void execute_scan(int angle, Scan_t* scan, int holdFlag){
    servo_move(angle, holdFlag); //scans and holds at angle
    scan->IRval = IRconversion((double)adc_read()); //IR values
    scan->sound_dist = ping_read(); //PING values
}
