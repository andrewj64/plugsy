#ifndef UART_H
#define UART_H
#include "stm32l476xx.h"

#include<string.h>

char msg[500];

void USART_Init(void);
//char check_uart(void);
int msg_ready(void);
void read_msg(void);

#endif
