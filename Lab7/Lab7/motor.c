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

void wind_up(int time)
{
	for(; time > 0; time--)
	{
		tick_up();
	}
}

void tick_up(void)
{
	uint8_t a, a0, b, b0;
	for(int j = 0; j < 512 / 60; j++)			// 64 steps x 5. deg = 360 deg
	{
		for(int i = 0; i < 4; i++)		// clockwise
		{
			for(int delay; delay < max; delay++);		// short delay
			a  = (fullStep[i] & 0x8) >> 3;
			b = (fullStep[i] & 0x4) >> 2;
			a0 = (fullStep[i] & 0x2) >> 1;
			b0 = (fullStep[i] & 0x1) >> 0;
			
			GPIOB->ODR &= ~0xCC;		// clear GPIOB 2,3,6,7
			GPIOB->ODR |= ( (a<<2) | (a0<<3) | (b<<6) | (b0<<7) );	// set GPIOB 2,3,6,7 with mask
			
		}
	}
}
	
void tick_down(void)
{
	uint8_t a, a0, b, b0;
	for(int j = 0; j < 512 / 60; j++)			// 64 steps x 5. deg = 360 deg
	{
		for(int i = 3; i > 0; i--)		// clockwise
		{
			for(int delay; delay < max; delay++);		// short delay
			a  = (fullStep[i] & 0x8) >> 3;
			b = (fullStep[i] & 0x4) >> 2;
			a0  = (fullStep[i] & 0x2) >> 1;
			b0 = (fullStep[i] & 0x1) >> 0;
			
			GPIOB->ODR &= ~0xCC;		// clear GPIOB 2,3,6,7
			GPIOB->ODR |= ( (a<<2) | (a0<<3) | (b<<6) | (b0<<7) );	// set GPIOB 2,3,6,7 with mask
			
		}
	}
}

void pulse(void)
{
	// toggle pulse signal
	for(int delay; delay < max; delay++){};
	GPIOB->ODR ^= GPIO_ODR_OD7;
	uint8_t on = (GPIOB->ODR & GPIO_ODR_OD7) >> 7;
	uint8_t print [2] = {0};
	print[0] = on + '0';
	LCD_DisplayString(print);
		
}

