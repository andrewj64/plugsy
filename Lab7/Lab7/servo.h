#include "stm32l476xx.h"
#include <stdint.h>

static volatile int pulse = 20; // 20 -> 0, 30 -> 45
void servo_init(void);
void set_servo(int pos);
void update_servo(void);
