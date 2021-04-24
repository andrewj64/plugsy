#include "servo.h"
#include "uart.h"
#include "Systick.h"

#include <stdio.h>
#include <string.h>

#define CONVERSION_FACTOR 20000/180
int high = 180;
int low = 0;
int clk = 0;
void servo_init(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;	//enable port clocks
	
	// configure PA0
	GPIOA->MODER &= ~0x0003U;			// clear MODER
	GPIOA->MODER |=  0x0001U;			// set to output 01
}

void set_servo(int pos) 
{
	
	//TIM3->CCR1 = 19999 + CONVERSION_FACTOR*pos;
	high = pos;
	low = 180 - pos;
}

void update_servo()
{
	if(clk == high)
	{
		GPIOA->ODR ^= GPIO_ODR_OD0;
	} 
	else if(clk >= (high+low))
	{
		clk = 0;
		GPIOA->ODR ^= GPIO_ODR_OD0;
	}
	clk++;
}

void TIM3_Init(void)
{
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;	// enable clock of timer 3
	TIM3->CR1 &= ~TIM_CR1_DIR;		// counting direction = up
	
	// timer driving frequency = 16 MHz/(1+PSC) = 2 MHz/(1+7) = 2 MHz
	// trigger frequency = 2 MHz/(1+ARR) = 2MHz/(1+3999) = 50 Hz
	TIM3->PSC  = 7;
	TIM3->ARR  = 39999;
	TIM3->CCR1 = 39999;		// duty ratio 50%
	TIM3->DIER |= TIM_DIER_UIE;
	
	NVIC_EnableIRQ(TIM3_IRQn);

	TIM3->CR1  |= TIM_CR1_CEN;		// enable timer dangit
	
	

}

void TIM3_IRQHandler()
{
	//GPIOA->ODR ^= GPIO_ODR_OD0;
	TIM3->SR &= ~TIM_SR_UIF; // clear flag
}

void servo_handler(void)
{
//	int length = strlen(msg);
//	char* pch;
//	int arg = 0;
//	char* buf = strdup(msg);
//	pch = strtok(buf," ");
//	while (pch != NULL)
//  {
//    pch = strtok (NULL, " ,.-");
//		if(arg == 1)
//		{
//			if(strcmp(pch, "0") == 0)	
//				close_servo();
//			else if(strcmp(pch, "1") == 0)
//				open_servo();
//		}
//		arg++;
//  }
//	free(buf);
}

void open_servo(void)
{
	pulse = 30;
	pulse_servo();
}

void close_servo(void)
{
	pulse = 20;
	pulse_servo();
}

