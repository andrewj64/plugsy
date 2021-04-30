#include "motor.h"
#include "LCD.h"
#include "servo.h"
#include "uart.h"

//TODO: These numbers are arbitrary. We need to test to discover the proper length.
#define LOOPS_FOR_INCH_X 450
#define LOOPS_FOR_INCH_Y 250
#define Z_DEPTH 500

int xPos = 0;
int yPos = 0;
//int zPos = 0;

int maxX;
int maxY;
int maxZ;
uint8_t fullStep[4] = {0x9, 0xA, 0x6, 0x5};

int getX(){
	return xPos;
}

int getY(){
	return yPos;
}

/*int getZ(){
	return zPos;
}*/

void motor_init(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOEEN;	//enable port clocks
	
	
	// configure PB 2,3,6,7
	GPIOB->MODER &= ~0xF0F0U;			// clear MODER
	GPIOB->MODER |=  0x5050U;			// set to output 01
	
	// set PUPDR to pull-down
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3 | GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD7);
	GPIOB->PUPDR |= (GPIO_PUPDR_PUPD2_1 | GPIO_PUPDR_PUPD3_1 | GPIO_PUPDR_PUPD6_1 | GPIO_PUPDR_PUPD7_1);
	
	//configure PE13,14,15 for Z, X, and Y directions respectively
	GPIOE->MODER &= ~(GPIO_MODER_MODE10 | GPIO_MODER_MODE14 | GPIO_MODER_MODE15);			// clear MODER
	GPIOE->MODER |=  (GPIO_MODER_MODE10_0 | GPIO_MODER_MODE14_0 | GPIO_MODER_MODE15_0);			// set to output 01
	
	GPIOE->PUPDR &= ~(GPIO_PUPDR_PUPD10 | GPIO_PUPDR_PUPD14 | GPIO_PUPDR_PUPD15);
	GPIOE->PUPDR |= (GPIO_PUPDR_PUPD10_1 | GPIO_PUPDR_PUPD14_1 | GPIO_PUPDR_PUPD15_1);
}

void setMotorPulseX(int speed)
{
	maxX = speed;
}

void setMotorPulseY(int speed)
{
	maxY = speed;
}

void setMotorPulseZ(int speed)
{
	maxZ = speed;
}

void pulseX(void)
{
	// toggle pulse signal
	//for(int delay = 0; delay < maxX; delay++);
	GPIOB->ODR ^= GPIO_ODR_OD7;
	for(int delay = 0; delay < maxX; delay++);
		
}

void pulseY(void)
{
	// toggle pulse signal
	for(int delay = 0; delay < maxY; delay++);
	GPIOB->ODR ^= GPIO_ODR_OD6;
	for(int delay; delay < maxY; delay++);
		
}

void pulseZ(void)
{
	// toggle pulse signal
	for(int delay = 0; delay < maxZ; delay++);
	GPIOB->ODR ^= GPIO_ODR_OD3;
	for(int delay; delay < maxZ; delay++);
		
}

void move_handler(){
	int params[2];
	get_params(2, params);
	
	//TODO:Add error checking to see if getting params worked and there were the right number.
	
	moveXY(params[0], params[1]);
}

void moveXY(int x, int y){
	//We could make this better by making in concurrent using a timer or systick or something
	
	int checkX = x;
	int checkY = y;
	xPos += x;
	yPos += y;
	//make it so it'll go negative
	if(checkX < 0){
		GPIOE->ODR |= GPIO_ODR_OD14;
		x = -x;
	} else {
		GPIOE->ODR &= ~GPIO_ODR_OD14;
	}
	if(checkY < 0){
		GPIOE->ODR |= GPIO_ODR_OD15;
		y = -y;
	} else {
		GPIOE->ODR &= ~GPIO_ODR_OD15;
	}
	
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
	
	//reset back to start moving position
//	if(checkX < 0){
//		GPIOE->ODR ^= GPIO_ODR_OD14;
//	}
//	if(checkY < 0){
//		GPIOE->ODR ^= GPIO_ODR_OD15;
//	}
	//TODO: do we want to add any error checking for motors?
}

void zero(){
	moveXY(-xPos, -yPos);
}

void plant(){
	int params[2];
	get_params(2, params);
	
	//TODO:Add error checking to see if getting params worked and there were the right number.
	
	moveXY(params[0], params[1]);
	
	//move down to depth
	for(int i = 0; i < Z_DEPTH; i++){
			pulseZ();
	}
	moveXY(-1,0);
	
	//open servo for just a sec
	setServo();

	moveXY(1,0);
	//change Z direction
	GPIOE->ODR |= GPIO_ODR_OD10;
	//move back up 
	for(int i = 0; i < Z_DEPTH; i++){
			pulseZ();
	}
	//change Z direction back so we always start by going down.
	GPIOE->ODR &= ~GPIO_ODR_OD10;
}

void weed(void){
	// 420
}
