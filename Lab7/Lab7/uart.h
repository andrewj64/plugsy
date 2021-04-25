#ifndef UART_H
#define UART_H
#include "stm32l476xx.h"

#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdbool.h>

static char msg[500];
static int msg_size = 0;

void USART_Init(void);
//char check_uart(void);
bool msg_ready(void);
void read_msg(void);
void handle_serial(void);
void print_uart(char* text);
void get_params(int num_params, int* params);

#endif
