#ifndef SYSTICK_H 
#define SYSTICK_H

#include "stm32l476xx.h"
#include <stdint.h>
#include <stdbool.h>

void SysTick_Init(void);
void SysTick_Handler(void);
void pulseServo(int pwm);

#endif
