#ifndef SERVO_H
#define SERVO_H
#include "stm32l476xx.h"
#include <stdint.h>

static volatile int pulse = 20; // 20 -> 0, 30 -> 45
void servo_init(void);
void set_servo(int pos);
void update_servo(void);
void servo_handler(void);
void open_servo(void);
void close_servo(void);
#endif
