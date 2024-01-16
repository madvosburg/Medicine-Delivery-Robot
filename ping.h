
#ifndef PING_H_
#define PING_H_

#include "timer.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "lcd.h"
#include "Timer.h"


volatile enum {LOW, HIGH, DONE} state;
volatile unsigned int rising_time;
volatile unsigned int falling_time;

//#define PING_FACTOR


void TIMER3B_Handler();

void ping_init();

void send_pulse();

int ping_read();

#endif
