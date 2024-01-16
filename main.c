
#include "Timer.h"
#include <math.h>
#include "lcd.h"
#include "scan.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "uart_extra_help.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "button.h"
#include "adc.h"
#include "movement.h"
#include "open_interface.h"
#include "lib.h"
#include "music.h"
#include "map.h"

volatile  char uart_data;
volatile  char flag;

/**
 * main.c
 */
int main(void)
{

        button_init();
        oi_t *cybot = oi_alloc();
        oi_init(cybot);
        timer_init();
        lcd_init();
        uart_init(115200);
        uart_interrupt_init();
        Scan_t scan;
        init_scan();
        load_song();


        int infraBuffer[181];
        float sonicBuffer[181];
        int quickSweepFlag = 0;

        int distanceTraveled = 0;
        float cbX = 410;
        float cbY = 15;
        setCybot(411, 15);


        char str[40];


        //Set cybot location
        setCybot(cbX, cbY);

        oi_setMotorCalibration(.88,.88);


        putty_string_print("\n\r\n\r\n\r\n\r\n");   //used for formatting to help see different runs, can take out of final project
        putty_string_print("Press 1 to start route to patient door ");



        while(!(uart_data == '9')){


            //forward
            if(uart_data == 'w'){
                distanceTraveled = move_forward2(cybot, 100);
                moveAngle(distanceTraveled);
                cbX = getPositionX();
                cbY = getPositionY();
                sprintf(str, "(%d, %d)", (int)cbX, (int)cbY);
                putty_string_print("\r");
                putty_string_print(str);
                uart_data = 'l';

            }

            //backward
            if(uart_data == 's'){
                distanceTraveled = move_backward(cybot, 100);
                moveAngle(distanceTraveled);
                cbX = getPositionX();
                cbY = getPositionY();
                sprintf(str, "(%d, %d)", (int)cbX, (int)cbY);
                putty_string_print("\r");
                putty_string_print(str);
                uart_data = 'l';

            }

            //rotate right
            if(uart_data == 'd'){
                rotate_clockwise(cybot, 30);
                uart_data = 'l';

            }

            //rotate left
            if(uart_data == 'a'){
                rotate_counterClockwise(cybot, 30);
                uart_data = 'l';

            }
            //Map field
            if(uart_data == 'm'){
                printField();
                uart_data = 'l';
            }

            if(uart_data == 'q'){
                quickSweepFlag = quick_sweep(&scan, 120);
                if(quickSweepFlag){
                    sprintf(str, "\n\rObject: %d degrees @ %d cm", quickSweepFlag, scan.IRval);
                    putty_string_print(str);
                }
                uart_data = 'l';
            }

            if(uart_data == 'e'){
                object_detection(&scan, infraBuffer, sonicBuffer, 180 );
                uart_data = 'l';
            }

            if(uart_data == '2'){
                putty_string_print("Arrived at door \n\r\n\r");
                patient_ID();
                uart_data = 'l';
            }


        }


            oi_free(cybot);

	return 0;
}

