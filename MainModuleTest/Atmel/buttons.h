/*
 * buttons.h
 *
 * ? Created: 23.10.2023 20:21:36
 *  Author: m4d
 */ 
#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "main.h"

#define PEAK_MODE_BUTTON_PIN PC0
#define TIME_PEAK_BUTTON_PIN PC1

#define MENU_BUTTON 2
#define SELECT_BUTTON 3

#define NUM_BUTTONS 2

void buttons_init();
void buttons_timer();
void on();
void off();
void off_timer();

struct Button {
	volatile unsigned char *PIN_REG;
	volatile unsigned char *PORT_REG;
	volatile unsigned char *DDR_REG;
	uint8_t pin;
	uint8_t wait_period;
	uint8_t pressed;
	void (*operation)();
};

volatile struct Button button_list[NUM_BUTTONS];

#endif /* BUTTONS_H_ */