#ifndef SERVO_H
#define SERVO_H
#include "stm32l476xx.h"
#include <stdint.h>

//static volatile int pulse = 30; // 20 -> 0, 30 -> 45
void servo_init(void);
void setServo(void);
void update_servo(void);
void servo_handler(void);
#endif
