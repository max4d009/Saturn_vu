/*
 * keyboard.h
 *
 * Created: 15.01.2026 2:18:32
 *  Author: max4d
 */ 


#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "main.h"

#define KEY_NONE 255

#define BUTTON_0 0
#define BUTTON_1 1
#define BUTTON_2 2

#define KEYBOARD_MODE_DEFAULT 0
#define KEYBOARD_MODE_BRIGHTNES 1

struct Button {
	uint8_t button_type;
	uint8_t is_pressed;
	uint8_t is_long_pressed;
	void (*operation)(uint8_t long_pressed);
};
struct Button buttons[2][3];

void keyboard_init();
void keys_update();

uint8_t get_key(uint16_t adc);

#endif /* KEYBOARD_H_ */