#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "uart_extra_help.h"
#include "math.h"

void moveAngle(int angle);

void mathCybot(float rawCmX, float rawCmY);

void setCybot(float cyBotX, float cyBotY);

void delZone(int dZX, int dZY);

int getPositionX();

int getPositionY();

int getAngle();

void setAngle(int changeAngle);

void printField();
