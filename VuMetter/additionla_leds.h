/*
 * additionla_leds.h
 *
 * Created: 24.01.2026 2:13:07
 *  Author: max4d
 */ 


#ifndef ADDITIONLA_LEDS_H_
#define ADDITIONLA_LEDS_H_

#include "main.h"

#define LED_PLUS_LEFT 0
#define LED_PLUS_RIGHT 1
#define LED_MINUS_LEFT 2
#define LED_MINUS_RIGHT 3
#define LED_NUM 4

#define LED_TYPE_PLUS 0
#define LED_TYPE_MINUS 1

#define PLUS_LED_TIME 3000
#define MINUS_LED_TIME 10000

void init_leds();
void additional_leds_update_ledTime();
void additional_leds_timer(uint8_t channel, uint16_t level);
void on_led_with_timer(uint8_t channel, uint8_t led_type);
void additional_led_timer();

struct Led {
	uint8_t pin;
	volatile uint8_t *PORT_REG;
	uint8_t timerOn;
	uint16_t timerStartTime;
	uint16_t led_time_ms;
	uint8_t channel;
	uint8_t type;
};
struct Led additional_leds[LED_NUM];

#endif /* ADDITIONLA_LEDS_H_ */