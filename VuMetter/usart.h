/*
 * usart.h
 *
 * Created: 20.01.2026 1:28:56
 *  Author: max4d
 */ 


#ifndef USART_H_
#define USART_H_

#include "main.h"

void USART_ini(unsigned int speed);
void USART_Transmit (unsigned char data);
void USART_send_var (char *name, char *value);
void USART_send_string(char *string);
void USART_send_int_var(char *name, int value);
void USART_send_2_int_var(char *name, int value, char *name2, int value2);
void IntToStr(long val, char str[]);


#endif /* USART_H_ */