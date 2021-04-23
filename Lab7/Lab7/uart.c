#include "uart.h"

static char msg[500];
static int msg_size = 0;

// Commands enum
typedef enum cmds {
  eQ,
  eSettings,
  eGetP,
  eSetLighting,
  eSetVLimit,
  eSetHLimit,
  eSetVSpeed,
  eSetHSpeed,
  eSetTracking,
  eRunD,
  eRunU,
  eMoveH,
  eMoveV,
  eEnd,
  eSetZero,
  eMoveS,
  eReset,
  eZeroV,
  eUnknown  
} cmd;

void USART_Init(void)
{
	//GPIO stuff
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;	//enable gpio clock
	
	GPIOD->MODER &= ~0x3C00;
	GPIOD->MODER |= 0x2800;
	
	GPIOD->AFR[0] |= 0x77 << (4*5);		// configure to UART alternate function 7
	
	GPIOD->OSPEEDR |= 0xF << 10;	// set to high speed
	
	GPIOD->PUPDR &= ~0x3C00;
	GPIOD->PUPDR |= 0x1400;			// select pull up
	
	GPIOD->OTYPER &= ~(3<<5);		// set to open drain
	
	//UART stuff
	
	// enable uart 1 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	
	// select sysclk as usart clock
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL;
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;	
	
	// Disable USART
	USART2->CR1 &= ~USART_CR1_UE;
	
	//set data length to 8 bits
	USART2->CR1 &= ~USART_CR1_M;
	
	//select stop bit
	USART2->CR2 &= ~USART_CR2_STOP;
	
	//set parity control as no parity
	USART2->CR1 &= ~USART_CR1_PCE;
	
	//oversampling by 16
	USART2->CR1 &= ~USART_CR1_OVER8;
	
	//set baud rate to 9600
	USART2->BRR = 0x1A0;//0x683U;
	
	//enable transmission and reception
	USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE);
	
	//enable usart
	USART2->CR1 |= USART_CR1_UE;
	
	//verify that usart is ready to transmit
	while((USART2->ISR & USART_ISR_TEACK) == 0);
	
	//verify that usart is ready to recieve
	while((USART2->ISR & USART_ISR_REACK) == 0);

}

int msg_ready(void)
{
	return (!(USART2->ISR & USART_ISR_RXNE)); //wait for hardware to set RXNE
}

void read_msg(void)
{
	char letter;
	while(letter != '\n')
	{
		while(!(USART2->ISR & USART_ISR_RXNE)); //wait for hardware to set RXNE
		letter = USART2->RDR; 									//reading RDR clears bit and sets to buffer
		msg_size++;
	}
	strcat(msg, "\0");
	msg_size++;
}

//cmd hashit(void)
//{
//	if(strcmp(msg, "?")) 						return eQ;
//  if(strcmp(msg, ".")) 						return eSettings;
//  if(strcmp(msg, "getP")) 				return eGetP;
//  if(strcmp(msg, "setLighting")) return eSetLighting;
//  if(strcmp(msg, "setVLimit")) return eSetVLimit;
//  if(strcmp(msg, "setHLimit")) return eSetHLimit;
//  if(strcmp(msg, "setVSpeed")) return eSetVSpeed; 
//  if(strcmp(msg, "setHSpeed")) return eSetHSpeed; 
//  if(strcmp(msg, "setTracking")) return eSetTracking;
//  if(strcmp(msg, "runD")) return eRunD;
//  if(strcmp(msg, "runU")) return eRunU;
//  if(strcmp(msg, "moveH")) return eMoveH;
//  if(strcmp(msg, "moveV")) return eMoveV;
//  if(strcmp(msg, "end")) return eEnd;
//  if(strcmp(msg,  "setZero")) return eSetZero;
//  if(strcmp(msg,  "zeroV")) return eZeroV;  // eZeroV and reset do the same thing right now
//  if(strcmp(msg,  "reset")) return eReset;
//  if(strcmp(msg, "moveS")) return eMoveS;
//  return eUnknown; // command not found
//}

void handle_serial(void)
{
//	switch(hashit()){
//    case eUnknown:
//    default:
//      break;     
//  }
//	strcpy(msg, "");
}

