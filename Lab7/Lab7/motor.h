#ifndef MOTOR_H 
#define MOTOR_H

#include "stm32l476xx.h"
#include <stdint.h>
#include <stdbool.h>

void motor_init(void);
void set_speed(int speed);
void pulseX(void);
void pulseY(void);
void pulseZ(void);

#endif
