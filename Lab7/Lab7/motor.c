#include "motor.h"
#include "LCD.h"

volatile int max;
uint8_t fullStep[4] = {0x9, 0xA, 0x6, 0x5};

void motor_init(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;	//enable port clocks
	
	// configure PB 2,3,6,7
	GPIOB->MODER &= ~0xF0F0U;			// clear MODER
	GPIOB->MODER |=  0x5050U;			// set to output 01
}

void set_speed(int speed)
{
	max = speed;
}

void pulseX(void)
{
	// toggle pulse signal
	for(int delay; delay < max; delay++){};
	GPIOB->ODR ^= GPIO_ODR_OD7;
	uint8_t on = (GPIOB->ODR & GPIO_ODR_OD7) >> 7;
		
}

void pulseY(void)
{
	// toggle pulse signal
	for(int delay; delay < max; delay++){};
	GPIOB->ODR ^= GPIO_ODR_OD6;
	uint8_t on = (GPIOB->ODR & GPIO_ODR_OD6) >> 6;
		
}

void pulseZ(void)
{
	// toggle pulse signal
	for(int delay; delay < max; delay++){};
	GPIOB->ODR ^= GPIO_ODR_OD3;
	uint8_t on = (GPIOB->ODR & GPIO_ODR_OD3) >> 3;
		
}

