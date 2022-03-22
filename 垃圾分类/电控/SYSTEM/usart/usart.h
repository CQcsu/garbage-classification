#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 	

void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart4_init(u32 bound);
#endif


