#include "movement.h"
#include "open_interface.h"
#include "lcd.h"
#include "math.h"
#include "uart_extra_help.h"
#include "map.h"



void putty_string_print2(char inputString[]) {
    int i;
    for(i=0; i < strlen(inputString); i++){
        uart_sendChar(inputString[i]);
    }
}

int move_forward2(oi_t *sensor , int distance){

     char str[20];
     int sum = 0;
     oi_setWheels(150, 150); // move forward; full speed
     while (sum < distance) {
         oi_update(sensor);
         sum += sensor->distance;
        // lcd_printf("Forward:%i   ,  %i", distance, sum);

         //////
        if(sensor->bumpLeft) {
            putty_string_print2("\n\rBump Left!");
            break;
        }
        if(sensor->bumpRight){
            putty_string_print2("\n\rBump Right!");
            break;
        }
        //////
        if((sensor->cliffLeftSignal > 3000)){
            putty_string_print2("\n\rEdge Left!");
            break;
        }
        if((sensor->cliffLeftSignal < 400)){
            putty_string_print2("\n\rCliff Left!");
            break;
        }
        //////
        if((sensor->cliffFrontLeftSignal > 2700)){
            putty_string_print2("\n\rEdge Front Left!");
            break;
        }
        if((sensor->cliffFrontLeftSignal < 400)){
            putty_string_print2("\n\rCliff Front Left!");
            break;
        }
        //////
        if((sensor->cliffFrontRightSignal > 2700)){
            putty_string_print2("\n\rEdge Front Right!");
            break;
        }
        if((sensor->cliffFrontRightSignal < 400)){
            putty_string_print2("\n\rCliff Front Right!");
            break;
        }
        //////
        if((sensor->cliffRightSignal > 3000)){
            putty_string_print2("\n\rEdge Right!");
            break;
        }
        if((sensor->cliffRightSignal < 400)){
            putty_string_print2("\n\rCliff Right!");
            break;
        }

     }

     oi_setWheels(0, 0); // stop

     sprintf(str, "\n\rForward: %d", sum);
     putty_string_print2(str);
     putty_string_print2("\n\r");
     return sum;
}

int move_backward(oi_t *sensor , int distance){
    char str[20];
    int sum = 0;
     oi_setWheels(-150, -150);
     while (sum < distance) {
         oi_update(sensor);
         sum += abs(sensor->distance);

     }
     oi_setWheels(0, 0); // stop
     sprintf(str, "Backward: %d", sum);
     putty_string_print2(str);
     putty_string_print2("\n\r");
     return sum;

}

//rotate right
void rotate_clockwise(oi_t *sensor, int degree){
    double degrees = (double)degree * .945;
    char str[20];
    double count = 0;
    oi_setWheels(-75,75);
    while(count > -degrees){
        oi_update(sensor);
        count+=sensor->angle;

    }
    setAngle(degree);
    sprintf(str, "%d degrees", getAngle());
    putty_string_print2("\n\r");
    putty_string_print2(str);
    oi_setWheels(0,0);
}



//rotate left
void rotate_counterClockwise(oi_t *sensor, int degree){
    double degrees = (double)degree * .945;
    char str[20];
    double count = 0;
    oi_setWheels(75,-75);
       while(count<degrees){
           oi_update(sensor);
           count+=sensor->angle;
           timer_waitMillis(10);

       }
       setAngle(-degree);
       sprintf(str, "%d degrees", getAngle());
       putty_string_print2("\n\r");
       putty_string_print2(str);
       oi_setWheels(0,0);
   }




