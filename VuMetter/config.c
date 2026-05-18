/*
 * config.c
 *
 * Created: 15.01.2026 2:35:07
 *  Author: max4d
 */ 

#include "config.h"


void update_parameters()
{
	for (uint8_t i = 0; i < BRIGHTNESS_CONFIG_COUNT; i++) {
		if (changed_parameters.brightness[i] != current.brightness[i]) {
			brightness_change(i, changed_parameters.brightness[i]);
			current.brightness[i] = changed_parameters.brightness[i];
			break;
		}
	}
	
	if (changed_parameters.peaks_mode != current.peaks_mode) {
		reset_vu_array();
		current.peaks_mode = changed_parameters.peaks_mode;
		start_anim(ANIM_TYPE_STARTUP, 1, TEXT_SATURN_ANIM_TYPE_DOT);
	}
	
	if (changed_parameters.peaks_time_id != current.peaks_time_id) {
		current.peaks_time_id = changed_parameters.peaks_time_id;
		additional_leds_update_ledTime();
		start_anim(ANIM_TYPE_PEAKS_TIME_CHANGED, 1, current.peaks_time_id+1);
	}
	
	if (changed_parameters.brightness_mode != current.brightness_mode) {
		current.brightness_mode = changed_parameters.brightness_mode;
		start_anim(ANIM_TYPE_BRIGHTNESS_CHANGED, 1, current.brightness_mode);
	}
	
	if (changed_parameters.saturn_text_on != current.saturn_text_on) {
		current.saturn_text_on = changed_parameters.saturn_text_on;
		if (current.saturn_text_on == 1) {
			sat1 = 0xFF;
			sat2 = 0xFF;
		} else {
			sat1 = 0x00;
			sat2 = 0x00;
		}
		soft_spi();
	}
}

void brightness_set_mode(uint8_t brightness_mode, uint8_t brightness_value)
{
	changed_parameters.brightness[brightness_mode] = brightness_value;	
}

void brightness_next_mode()
{
	changed_parameters.brightness_mode++;
	if (changed_parameters.brightness_mode >= BRIGHTNESS_CONFIG_COUNT) {
		changed_parameters.brightness_mode = BRIGHTNESS_BACKLIGHT_CONFIG;
	}
}

uint8_t get_peaks_mode()
{
	return current.peaks_mode;
}

uint8_t get_peaks_time_id()
{
	return current.peaks_time_id;
}

void set_peaks_mode(uint8_t mode)
{
	changed_parameters.peaks_mode = mode;
}

void set_peaks_time_id(uint8_t time_id)
{
	changed_parameters.peaks_time_id = time_id;
}

void peaks_next_mode()
{
	changed_parameters.peaks_mode++;
	if (changed_parameters.peaks_mode >= PEAKS_MODE_COUNT) {
		changed_parameters.peaks_mode = PEAKS_MODE_OFF;
	}	
}

void peaks_next_time()
{
	changed_parameters.peaks_time_id++;
	if (changed_parameters.peaks_time_id >= 12) {
		changed_parameters.peaks_time_id = 0;
	}    
}


void brightness_change(uint8_t brightness_mode, uint8_t brightness)
{
	if (brightness_mode == BRIGHTNESS_BACKLIGHT_CONFIG) {
		if (brightness == 1) {
			PORTB |= (1 << BACKLIGHT_R_BRIGHT_PIN_PB7);
			sat3 |= (1 << HC595_3_BACKLIGHT_L_BRIGHT_PIN_6);
		} else {
			PORTB &= ~(1 << BACKLIGHT_R_BRIGHT_PIN_PB7);
			sat3 &= ~(1 << HC595_3_BACKLIGHT_L_BRIGHT_PIN_6);
		}
	}
	
	if (brightness_mode == BRIGHTNESS_VU_CONFIG) {
		start_anim(ANIM_TYPE_SHOW_VU, 0);
		if (brightness == 1) {
			PORTB |= (1 << VU_PEAK_R_BRIGHT_PIN_PB6);
			sat4 |= (1 << HC595_4_VU_PEAK_L_BRIGHT_PIN_6);
		} else {
			PORTB &= ~(1 << VU_PEAK_R_BRIGHT_PIN_PB6);
			sat4 &= ~(1 << HC595_4_VU_PEAK_L_BRIGHT_PIN_6);
		}
	}
	
	if (brightness_mode == BRIGHTNESS_TEXT_CONFIG) {
		if (brightness == 1) {
			PORTB |= (1 << TEXT_R_BRIGHT_PIN_PB1);
			sat4 |= (1 << HC595_4_TEXT_R_BRIGHT_PIN_5);
		} else {
			PORTB &= ~(1 << TEXT_R_BRIGHT_PIN_PB1);
			sat4 &= ~(1 << HC595_4_TEXT_R_BRIGHT_PIN_5);
		}
		
		if (current.saturn_text_on == 1) {
			sat1 = 0xFF;
			sat2 = 0xFF;
		} else {
			sat1 = 0x00;
			sat2 = 0x00;
		}
	}
	soft_spi();
}