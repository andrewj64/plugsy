#ifndef MOTOR_H 
#define MOTOR_H

#include "stm32l476xx.h"
#include <stdint.h>
#include <stdbool.h>

static int pos[3] = {0,0,0};

void motor_init(void);
void set_speed(int speed);
void pulseX(void);
void pulseY(void);
void pulseZ(void);

#endif
