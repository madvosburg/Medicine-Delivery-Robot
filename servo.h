#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/interrupt.h"

void servo_init(void);

int servo_move(float degrees, int holdFlag);
