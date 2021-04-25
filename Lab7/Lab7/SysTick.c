#include "Systick.h"
#include "servo.h"

#define PERIOD 400
volatile int count = 0;
volatile int pulses = PERIOD;
volatile int pulse = 30; // 20 -> 0, 30 -> 45



void SysTick_Init(void) {
SysTick->CTRL = 0; 						// Disable SysTick
	
SysTick->LOAD = 200; 			// Set reload register .1 ms period 
	
// Set interrupt priority of SysTick to least urgency (i.e., largest priority value)
NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	
SysTick->VAL = 0; 						// Reset the SysTick counter value
	
// Select processor clock: 1 = processor clock; 0 = external clock
SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	
// Enables SysTick interrupt, 1 = Enable, 0 = Disable
SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
// Enable SysTick
SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
	if(pulses < 20)
	{
		if(count < pulse)	{
			GPIOA->ODR |= GPIO_ODR_OD0;
		}	else {
			//go CAN_TSR_LOW//
			GPIOA->ODR &= ~GPIO_ODR_OD0;
		}
		
		count++;
		if(count > PERIOD)
		{
			count = 0;
			pulses++;
		}
	}
}

void pulseServo(int pwm)
{
	pulse = pwm;
	pulses = 0;
	count = 0;
}

//void setSec(uint32_t countTime)
//{
//	sec = countTime;
//	count = true;
//}


