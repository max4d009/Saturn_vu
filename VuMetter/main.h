/*
 * main.h
 *
 * λ Created: 12.07.2021 0:32:36
 *  Author: m4d
 */ 
#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <compat/twi.h>
#include <avr/io.h>

#include "m4d_adc.h"
#include "i2c.h"
#include "eeprom.h"
#include "software_spi.h"
#include "keyboard.h"
#include "config.h"
#include "anim.h"
#include "vu.h"
#include "additionla_leds.h"
#include "timer1.h"

#define CLK PB2 // clock 14
#define DS PB5  // data 17
#define E PB4   // Enter 16
#define VU_PEAK_R_BRIGHT_PIN_PB6 PB6
#define BACKLIGHT_R_BRIGHT_PIN_PB7 PB7
#define TEXT_R_BRIGHT_PIN_PB1 PB1

#define VU_20DB_MK_PIN_PC2 PC2
#define MINUS_MK_PIN_PC3 PC3
#define PLUS_MK_PIN_PB0 PB0

#define HC595_4_MINUS_PIN_0 0
#define HC595_3_PLUS_PIN_5 5

#define HC595_3_BACKLIGHT_L_BRIGHT_PIN_6 6
#define HC595_4_VU_PEAK_L_BRIGHT_PIN_6 6
#define HC595_4_TEXT_R_BRIGHT_PIN_5 5


#define LEFT_CHANNEL 0
#define RIGHT_CHANNEL 1

#define PEAKS_MODE_OFF 0
#define PEAKS_MODE_SOLID 1
#define PEAKS_MODE_BAR  2
#define PEAKS_MODE_DOT  3
#define PEAKS_MODE_COUNT 4

#define BRIGHTNESS_BACKLIGHT_CONFIG 0
#define BRIGHTNESS_VU_CONFIG 1
#define BRIGHTNESS_TEXT_CONFIG 2
#define BRIGHTNESS_CONFIG_COUNT 3

#define TEXT_SATURN_ANIM_TYPE_SOLID  0
#define TEXT_SATURN_ANIM_TYPE_DOT 1

#define VU_LEDS 9

struct CurrentParameters {
	uint8_t peaks_mode;
	uint8_t peaks_time_id;
	uint8_t brightness[BRIGHTNESS_CONFIG_COUNT];
	uint8_t brightness_mode;
	uint8_t saturn_text_on;
};
struct CurrentParameters current;
struct CurrentParameters changed_parameters;


#endif /* MAIN_H_ */