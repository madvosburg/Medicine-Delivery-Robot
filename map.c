#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "uart_extra_help.h"
#include "math.h"

#define XLEN 14
#define YLEN 8
#define PI 3.141592653589793

bool map[XLEN][YLEN] = {0};
int oldCyBotX = 0;
int oldCyBotY = 0;
float cybotX = 0;
float cybotY = 0;
int delZoneX = 14;
int delZoneY = 8;
int angle = 180;


//only call on init
void setCybot(float cyBotX, float cyBotY){

    cybotX = cyBotX;
    cybotY = cyBotY;

    map[oldCyBotX][oldCyBotY] = 0;
    map[(int)cyBotX][(int)cyBotY] = 1;

    oldCyBotX = cybotX;
    oldCyBotY = cybotY;
}



//Need to make a function that calls location and and angle (angle % 90) to do trig
//then calculate x and y straight lengths bases on hypot movement
//call get posX and Y after to set main pos right
void moveAngle(int distanceMoved){

    // Get the distance in cm not mm
    int distMoveCM = (distanceMoved/10);
    // gets an angle between 0-90 regardless of direction
    int hypoAngle = (angle % 90);
    //gets rad angle for calc
    double radAngle = (hypoAngle * (PI/180.0));
    //Find distance moved x
    double sideA = (distMoveCM * (cos(radAngle)));
    //Find distance moved y
    double sideB = (distMoveCM * (sin(radAngle)));


    //quad 1
    if((angle >= 0 || angle >= 360) && angle < 90){

        cybotX += sideB;
        cybotY -= sideA;
    }

    //quad 2
    if(angle >= 90 && angle < 180){

        cybotX += sideA;
        cybotY += sideB;

    }

    //quad 3
    if(angle >= 180 && angle < 270){

        cybotX -= sideB;
        cybotY += sideA;
    }

    //quad 4
    if(angle >= 270 && (angle < 360 || angle < 0)){

        cybotX -= sideA;
        cybotY -= sideB;
    }




    //should be the other place this is called besides init it
    setCybot(cybotX, cybotY);

}


//////////////////////////////////////////Rounding bug is in mathCybot


//takes cm cord divdes by 2.54 to get inches then takes (fieldTotal / convertedCord) to get map location for XY
void mathCybot(float rawCmX, float rawCmY){
    //cm to in for map
    float rawInX = (rawCmX/2.54);
    float rawInY = (rawCmY/2.54);
    // float number between 0-1 that indicates how far progress in a 12 X 12 portion of tile it is
    float mediumRareInX = (rawInX / (XLEN * 12));
    float mediumRareInY = (rawInY / (YLEN * 12));
    //scales to map to get subtile the CyBot is on
    int wellDoneInX = (int)((mediumRareInX) * XLEN);
    int wellDoneInY = (int)((mediumRareInY) * YLEN);


    setCybot(wellDoneInX, wellDoneInY);
}

void delZone(int dZX, int dZY){

    delZoneX = dZX;
    delZoneY = dZY;

}

int getPositionX(){
    return cybotX;
}

int getPositionY(){
    return cybotY;
}

int getAngle(){
    return angle;
}

//This needs to be the only place angle is updated!!!!
void setAngle(int changeAngle){
    angle += changeAngle;
    //rotates to 360/0
    if(angle >= 360){
        angle = (angle - 360);
    }
    //rotates to 0/360
    if(angle <= 0){
        angle = (angle + 360);
    }
}


void printField(){
    int i; int j;
    char sendChar = '0';

    uart_sendChar('\n');
    uart_sendChar('\r');
    for(i = 0; i < YLEN; i++){
        for(j = 0; j < XLEN; j++ ){

            if(map[j][i] == 1){
                sendChar = '1';
            }
            else{
                sendChar = '0';
            }

            if(i == delZoneY && j == delZoneX){
                sendChar = '1';
            }

            uart_sendChar(sendChar);
            uart_sendChar(' ');
        }
        uart_sendChar('\n');
        uart_sendChar('\r');
    }

}


