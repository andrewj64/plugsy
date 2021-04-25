#include "uart.h"
#include "motor.h"
#include "servo.h"
#include "LCD.h"
#include "soil.h"

bool newCommand = true;

// Commands enum
typedef enum cmds {
  eQ,
  eSettings,
  eGetP,	// getP (prints position)
  eZero,	// zero x, y, and z
	eMove,	// move <x> <y> [z]
	eServo,	// moveServo <0 | 1> (closed or open)
	ePlant, // plant <x> <y>
	eWeed,  // weed <x> <y>
	eWater, // water <x> <y>
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

bool msg_ready(void)
{
	if(newCommand)
	{
		print_uart("> ");
		newCommand = false;
	}
	return (!(USART2->ISR & USART_ISR_RXNE)); //wait for hardware to set RXNE
}

void read_msg(void)
{
	char letter;
	while(letter != '\r')
	{
		while(!(USART2->ISR & USART_ISR_RXNE)); //wait for hardware to set RXNE
		letter = USART2->RDR; 									//reading RDR clears bit and sets to buffer
		USART2->TDR = letter;		// write to TDR
		while(!(USART2->ISR & USART_ISR_TC)); 	// wait until TC bit is set
		USART2->ICR |= USART_ICR_TCCF;	// clear TC flag
		if(letter != '\r')
			strcat(msg, &letter);
		else
			strcat(msg, "\0");
		msg_size++;
	}
	print_uart("\r\n");
}

void print_uart(char* text)	// for putty, add \r\n to end of messages to display correctly
{
	while(!(USART2->ISR & USART_ISR_TXE));	// wait until hardware sets TXE
	int length = strlen(text);
	for(int i = 0; i < length; i++)
	{
		USART2->TDR = text[i];		// write to TDR
		for(int delay = 0; delay < 1000; delay++);	// give UART time to print
	}
	while(!(USART2->ISR & USART_ISR_TC)); 	// wait until TC bit is set
	
	USART2->ICR |= USART_ICR_TCCF;	// clear TC flag
}

cmd hashit(void)
{
	//char* tmp = msg;
	// Check if command string contains
	if(strstr(msg, "?")) 					return eQ;
  if(strstr(msg, ".")) 					return eSettings;
  if(strstr(msg, "getP")) 			return eGetP;
	if(strstr(msg, "zero")) 			return eZero;
	if(strstr(msg, "move")) 			return eMove;
	if(strstr(msg, "servo")) 	return eServo;
	if(strstr(msg, "plant")) 			return ePlant;
  return eUnknown; // command not found
}

void handle_serial(void)
{
	char* tmp = msg;
	print_uart("CMD: ");
	print_uart(msg);
	print_uart("\r\n");
	cmd hash_cmd = hashit();
	switch(hash_cmd){
		case eQ:
			//print_commands();
			break;
		case eSettings:
			//print_settings();
			break;
		case eGetP:
			//print_uart(get_position());
			break;
		case eZero:
			zero();
			break;
		case eMove:
			move_handler();
			break;
		case eServo:
			setServo();
			break;
		case ePlant:
			plant();
			break;
		case eWeed:
			//weed();
			break;
		case eWater:
			water();
			break;
    case eUnknown:
    default:
			print_uart("Command not found\r\n");
      break;     
  }
	strcpy(msg, "");
	newCommand = true;
}

void get_params(int num_params, int* params)
{
	int length = strlen(msg);
	char* pch;
	char buf[100];
	strncpy(buf, msg, sizeof(buf));
	pch = strtok(buf," ,");	// get rid of command
	for(int p = 0; p < num_params; p++)
	{
		pch = strtok(NULL," ,");	
		if(pch == NULL)
		{
			if(p < num_params-1)
				print_uart("Failed to get correct number of parameters\r\n");
			break;
		}
		params[p] = atoi(pch);
	}
	
}
