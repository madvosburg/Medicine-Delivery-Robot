/*
 * lib.c
 *
 *  Created on: Dec 4, 2023
 *      Author: mvosburg
 */

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
#include "open_interface.h"
#include "movement.h"
#include "music.h"
#include "lib.h"
#include "uart_extra_help.h"
#include "button.h"


int right_angle = 90;
//prints string to console, 192.168.1.1
void putty_string_print(char inputString[]) {
    int i;
    for(i=0; i < strlen(inputString); i++){
        uart_sendChar(inputString[i]);
    }
}

//Patient ID input with buttons and updates putty
void patient_ID(){

    int password_flag = 0;  //keeps track if the password has been entered
    uint8_t button = 0;     //keeps track of what button is pressed

    putty_string_print("Update Status: Arrived at door \n\r\n\r");
    oi_play_song(0);
    lcd_printf("Input ID #:");
    timer_waitMillis(300);

    //patient ID = 123
    while(password_flag == 0){          //continues until full password is correct
        button = button_getButton();        //gets first button pressed (needs to hold until see number on lcd)
        lcd_printf("Input ID #: %u", button);           //prints held button
        timer_waitMillis(2000);
        if(button == 1){                        //if first button = 1, then first key correct
            button = button_getButton();        //get next button pressed (needs to be held until number is seen on lcd)
            lcd_printf("Input ID #: %u", button);           //prints new held button
            timer_waitMillis(2000);
                if(button == 2){                        //if second button = 1, then second key correct
                    button = button_getButton();    //gets next button pressed (needs to be held until number is seen on lcd)
                    lcd_printf("Input ID #: %u", button);       //prints last held button
                    timer_waitMillis(2000);
                        if(button == 3){        //if last button = 3, then third key correct
                            password_flag = 1;      //set flag because password is correct, continue with rest of program
                        }
                }
        }
    }

    lcd_printf("Dispensing meds");
    password_flag = 0;      //resets password flag for next door
    putty_string_print("Patient #123 has received their medicine \r\n");
}

  typedef struct objectStruct{
    int startPoint;
    int endPoint;
    float centerDistance;
} objStruct;

extern volatile objStruct objects[10] = {-1};

//counts how many objects detected
int objectCount(){
    int i;
    for(i = 0; i < 10; i++){ //iterates over each array element until an empty one is found
        if(objects[i].startPoint == -1){
            return(i);
        }
    }
    return(i);
}

//keeps track of distance values
void populate_array(Scan_t *scan, int infraBuffer[], float sonicBuffer[], int sweepWidth){ //fills IR and sonic buffers with data
    int i = 0;
    for(i = (90 - (sweepWidth / 2)); i <= (90 + (sweepWidth / 2)); i++){
        execute_scan(i, scan, 1);
        timer_waitMillis(40);
        lcd_printf("%d", scan->IRval);  //testing distances during scan
        infraBuffer[i] = scan->IRval;
        sonicBuffer[i] = scan->sound_dist;

    }
}

//quick sweep to detect objects while moving
int quick_sweep(Scan_t *scan, int sweepWidth){
    int i = 0;
    for(i = (90 - (sweepWidth / 2)); i <= (88 + (sweepWidth / 2)); i = i+3){
        execute_scan(i, scan, 1);
        if(scan->IRval < 40){

            return i;
        }
    }

    return(0);
}

//int infraBuffer[], float sonicBuffer[]
void findCenterDistance(float sonicBuffer[]){
        int centerAngle; int i = 0;
    for (i = 0; i < objectCount(); i++){
        centerAngle = (objects[i].endPoint + objects[i].startPoint) / 2;
        objects[i].centerDistance = sonicBuffer[centerAngle];
    }
}


//tracks objects if detected by quick sweep
void object_detection(Scan_t *scan, int infraBuffer[], float sonicBuffer[], int sweepWidth){
    int i = 0; int k = 0; char objFlag = 0; char printBuffer[64];
    clearStruct();

    populate_array(scan, infraBuffer, sonicBuffer, sweepWidth); //fills array for processing

    for(i = (90 - (sweepWidth / 2)); i <= (88 + (sweepWidth / 2)); i++){ // differentiates objects
        if((((abs(infraBuffer[i] - infraBuffer[i+1])) > 20) && ((abs(infraBuffer[i] - infraBuffer[i+2])) > 35))){ // IF distance between last point and current point > 15 [AND distance < 80]...

            //detects new object
            if((!objFlag) && (infraBuffer[i] < 60)){
                objects[k].startPoint = i;
                objFlag = 1; //awaiting end-point detection...

            }


            //declares end of object
            else if(objFlag){
                objects[k].endPoint = i;
                k++; //increment object
                objFlag = 0; //new object can be declared

            }
        }
    }

    objects[k].endPoint = i; //fixes bug where you can get negative angle-- this will have no effect on object n+1
    findCenterDistance(sonicBuffer);
    info_dump();        //take out in final code

}

//resets struct objects[], returns # of objects deleted
//technically you only need to set startPoint to -1, but that might fuck something up so i reset them all
int clearStruct(){
    int i;
    int count = objectCount();
    for(i = 0; i < 10; i++){
        objects[i].startPoint = -1;
        objects[i].endPoint = -1;
        objects[i].centerDistance = -1;
    }
    return count;
}


//take out of final code, prints to putty
void info_dump(){
    int i = 0;
        char printBuffer[64];
        int count = objectCount(); // like "sizeof" but for structs

        sprintf(printBuffer, "\n\n\rThere are %d objects.\r", count);
        putty_string_print(printBuffer);

        //iterates over every object to print its distance and angle
        for(i = 0; i < count; i++){

            sprintf(printBuffer, "\n\rObject %d, angle: %d, %d, cm = %.2lf", (i+1), objects[i].startPoint, objects[i].endPoint, objects[i].centerDistance); //prints distance
            putty_string_print(printBuffer);

        }
 }

