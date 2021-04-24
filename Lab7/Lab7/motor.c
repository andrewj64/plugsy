#include "motor.h"
#include "LCD.h"

//TODO: These numbers are arbitrary. We need to test to discover the proper length.
#define LOOPS_FOR_INCH_X 1000
#define LOOPS_FOR_INCH_Y 1000

int xPos = 0;
int yPos = 0;
int zPos = 0;

volatile int max;
uint8_t fullStep[4] = {0x9, 0xA, 0x6, 0x5};

int getX(){
	return xPos;
}

int getY(){
	return yPos;
}

int getZ(){
	return zPos;
}

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

void move_handler(){
	int params[2];
	get_params(2, params);
	
	//TODO:Add error checking to see if getting params worked and there were the right number.
	
	moveXY(params[0], params[1]);
}

void moveXY(int x, int y){
	//We could make this better by making in concurrent using a timer or systick or something
	xPos += x;
	yPos += y;
	//Move X inches
	for(int i = 0; i < x; i++){
		for(int j = 0; j < LOOPS_FOR_INCH_X; j++){
			pulseX();
		}
	}
	//move y inches
	for(int i = 0; i < y; i++){
		for(int j = 0; j < LOOPS_FOR_INCH_Y; j++){
			pulseY();
		}
	}
	//TODO: do we want to add any error checking for motors?
}