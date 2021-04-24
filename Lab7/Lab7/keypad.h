#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32l476xx.h"
#include <stdint.h>

unsigned char keypad_scan(void);
void keypad_pin_init(void);
#endif
