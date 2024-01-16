

#include <stdio.h>
#include <math.h>
#include "Timer.h"
#include "open_interface.h"
#include "uart_extra_help.h"


void putty_string_print2(char inputString[]);

int move_forward2(oi_t *sensor , int distance);

int move_backward(oi_t *sensor , int distance);

//rotate right
void rotate_clockwise(oi_t *sensor, int degrees);

//rotate left
void rotate_counterClockwise(oi_t *sensor, int degrees);


void detectEdge(oi_t *sensor, int right_angle, int *remaining);

