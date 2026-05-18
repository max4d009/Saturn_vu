/*
 * additional_leds.c
 *
 * Created: 24.01.2026 2:12:53
 *  Author: max4d
 */ 
#include "additionla_leds.h"


void init_leds()
{
	additional_leds[LED_PLUS_LEFT].timerOn = 0;
	additional_leds[LED_PLUS_LEFT].timerStartTime = 0;
	additional_leds[LED_PLUS_LEFT].pin = PLUS_MK_PIN_PB0;
	additional_leds[LED_PLUS_LEFT].PORT_REG = &PORTB;
	additional_leds[LED_PLUS_LEFT].channel = LEFT_CHANNEL;
	additional_leds[LED_PLUS_LEFT].type = LED_TYPE_PLUS;
	
	additional_leds[LED_PLUS_RIGHT].timerOn = 0;
	additional_leds[LED_PLUS_RIGHT].timerStartTime = 0;
	additional_leds[LED_PLUS_RIGHT].pin = HC595_3_PLUS_PIN_5;
	additional_leds[LED_PLUS_RIGHT].PORT_REG = &sat3;
	additional_leds[LED_PLUS_RIGHT].channel = RIGHT_CHANNEL;
	additional_leds[LED_PLUS_RIGHT].type = LED_TYPE_PLUS;
	
	additional_leds[LED_MINUS_LEFT].timerOn = 0;
	additional_leds[LED_MINUS_LEFT].timerStartTime = 0;
	additional_leds[LED_MINUS_LEFT].pin = MINUS_MK_PIN_PC3;
	additional_leds[LED_MINUS_LEFT].PORT_REG = &PORTC;
	additional_leds[LED_MINUS_LEFT].channel = LEFT_CHANNEL;
	additional_leds[LED_MINUS_LEFT].type = LED_TYPE_MINUS;
	
	additional_leds[LED_MINUS_RIGHT].timerOn = 0;
	additional_leds[LED_MINUS_RIGHT].timerStartTime = 0;
	additional_leds[LED_MINUS_RIGHT].pin = HC595_4_MINUS_PIN_0;
	additional_leds[LED_MINUS_RIGHT].PORT_REG = &sat4;
	additional_leds[LED_MINUS_RIGHT].channel = RIGHT_CHANNEL;
	additional_leds[LED_MINUS_RIGHT].type = LED_TYPE_MINUS;
	
	additional_leds_update_ledTime();
}

void additional_leds_update_ledTime()
{
	additional_leds[LED_PLUS_LEFT].led_time_ms = peaks_times_ms[current.peaks_time_id] * 10;
	additional_leds[LED_PLUS_RIGHT].led_time_ms = peaks_times_ms[current.peaks_time_id] * 10;
	additional_leds[LED_MINUS_LEFT].led_time_ms = peaks_times_ms[current.peaks_time_id] * 30;
	additional_leds[LED_MINUS_RIGHT].led_time_ms = peaks_times_ms[current.peaks_time_id] * 30;
}

void on_led_with_timer(uint8_t channel, uint8_t led_type)
{
	for (uint8_t i = 0; i < LED_NUM; i++) {
		if (additional_leds[i].type == led_type && additional_leds[i].channel == channel) {
			
			additional_leds[i].timerStartTime = get_system_ms();
			*additional_leds[i].PORT_REG |= (1 << additional_leds[i].pin);
			
			if (additional_leds[i].channel == RIGHT_CHANNEL && additional_leds[i].timerOn == 0) {
				soft_spi();
			}
			
			additional_leds[i].timerOn = 1;
		}
	}
}

void additional_led_timer()
{
	for (uint8_t i = 0; i < LED_NUM; i++) {
		if (additional_leds[i].timerOn == 0) {
			continue;
		}
		
		if (has_time_elapsed(additional_leds[i].timerStartTime, additional_leds[i].led_time_ms)) {
			additional_leds[i].timerOn = 0;
			additional_leds[i].timerStartTime = 0;
			*additional_leds[i].PORT_REG &= ~(1 << additional_leds[i].pin);
			
			if (additional_leds[i].channel == RIGHT_CHANNEL) {
				soft_spi();
			}
		}
	}
}

void additional_leds_timer(uint8_t channel, uint16_t level)
{
	if (channel == LEFT_CHANNEL) {
		if (level > 958) {
			on_led_with_timer(LEFT_CHANNEL, LED_TYPE_PLUS);
		}
		if (level > 10) {
			on_led_with_timer(LEFT_CHANNEL, LED_TYPE_MINUS);
		}
	}
	if (channel == RIGHT_CHANNEL) {
		if (level > 958) {
			on_led_with_timer(RIGHT_CHANNEL, LED_TYPE_PLUS);
		}
		if (level > 10) {
			on_led_with_timer(RIGHT_CHANNEL, LED_TYPE_MINUS);
		}
	}
}