/*
 * anim.c
 *
 * Created: 23.01.2026 3:19:47
 *  Author: max4d
 */ 
#include "anim.h"

static void change_peak_time_anim();
static void brightness_mode_anim();
static void startup_anim();
static void saturn_text_anim(uint8_t channel);
static void show_vu_anim();

void anim_timer()
{
	if (anim_parameters[ANIM_TYPE_PEAKS_TIME_CHANGED].started == 1) {
		change_peak_time_anim();
	}
	if (anim_parameters[ANIM_TYPE_BRIGHTNESS_CHANGED].started == 1) {
		brightness_mode_anim();
	}
	if (anim_parameters[ANIM_TYPE_STARTUP].started == 1) {
		startup_anim();
	}
	if (anim_parameters[ANIM_SATURN_TEXT_LEFT].started == 1) {
		saturn_text_anim(LEFT_CHANNEL);
	}
	if (anim_parameters[ANIM_SATURN_TEXT_RIGHT].started == 1) {
		saturn_text_anim(RIGHT_CHANNEL);
	}
	if (anim_parameters[ANIM_TYPE_SHOW_VU].started == 1) {
		show_vu_anim();
	}
}

void start_anim(uint8_t anim_type, uint8_t param_count, ...)
{
	if (param_count > ANIM_MAX_PARAMS) param_count = ANIM_MAX_PARAMS;
	
	va_list args;
	va_start(args, param_count);
	
	for (uint8_t i = 0; i < ANIM_MAX_PARAMS; i++) {
		anim_parameters[anim_type].parameters[i] = 0;
	}
	
	for (uint8_t i = 0; i < param_count; i++) {
		anim_parameters[anim_type].parameters[i] = (uint8_t)va_arg(args, int);
	}

	anim_parameters[anim_type].started_time_ms = get_system_ms();
	anim_parameters[anim_type].current_frame = 0;
	anim_parameters[anim_type].started = 1;
}

static void change_peak_time_anim()
{
	uint8_t leds_on_count = anim_parameters[ANIM_TYPE_PEAKS_TIME_CHANGED].parameters[0];
	
	sat1 = 0x00;
	sat2 = 0x00;
	
	// Ограничение
	if (leds_on_count > 12) leds_on_count = 12;
		
	// Просто устанавливаем биты
	for (uint8_t i = 1; i <= leds_on_count; i++) {
		if (i < 7) {
			// Первые 6 светодиодов на sat1
			sat1 |= (1 << (7 - i));
		} else {
			// Следующие 6 на sat2
			sat2 |= (1 << (7 - (i - 7)));
		}
	}
	
	soft_spi();
	
	if (!has_time_elapsed(anim_parameters[ANIM_TYPE_PEAKS_TIME_CHANGED].started_time_ms, 1000)) {
		return;
	}
	
	sat1 = 0x00;
	sat2 = 0x00;
	soft_spi();
	
	anim_parameters[ANIM_TYPE_PEAKS_TIME_CHANGED].started = 0;
	if (current.saturn_text_on == 1) {
		sat1 = 0xFF;
		sat2 = 0xFF;
		soft_spi();
	}
}

static void brightness_mode_anim()
{
	if (!has_time_elapsed(anim_parameters[ANIM_TYPE_BRIGHTNESS_CHANGED].started_time_ms, 100)) {
		return;
	}		
	
	uint8_t *current_frame = &anim_parameters[ANIM_TYPE_BRIGHTNESS_CHANGED].current_frame;
	uint8_t brightness_mode = anim_parameters[ANIM_TYPE_BRIGHTNESS_CHANGED].parameters[0];
	
	
	if (brightness_mode == BRIGHTNESS_BACKLIGHT_CONFIG) {
		if ((*current_frame) % 2) {
			PORTB |= (1 << BACKLIGHT_R_BRIGHT_PIN_PB7);
			sat3 |= (1 << HC595_3_BACKLIGHT_L_BRIGHT_PIN_6);
		} else {
			PORTB &= ~(1 << BACKLIGHT_R_BRIGHT_PIN_PB7);
			sat3 &= ~(1 << HC595_3_BACKLIGHT_L_BRIGHT_PIN_6);
		}
	}
	
	if (brightness_mode == BRIGHTNESS_VU_CONFIG) {
		//start_anim(ANIM_TYPE_SHOW_VU, 0);
		if ((*current_frame) % 2) {
			shoe_all_vu();
			
			PORTB |= (1 << VU_PEAK_R_BRIGHT_PIN_PB6);
			sat4 |= (1 << HC595_4_VU_PEAK_L_BRIGHT_PIN_6);
		} else {
			PORTB &= ~(1 << VU_PEAK_R_BRIGHT_PIN_PB6);
			sat4 &= ~(1 << HC595_4_VU_PEAK_L_BRIGHT_PIN_6);
		}
	}
	
	if (brightness_mode == BRIGHTNESS_TEXT_CONFIG) {
		if ((*current_frame) % 2) {
			PORTB |= (1 << TEXT_R_BRIGHT_PIN_PB1);
			sat4 |= (1 << HC595_4_TEXT_R_BRIGHT_PIN_5);
			sat1 = 0xFF;
			sat2 = 0xFF;
		} else {
			PORTB &= ~(1 << TEXT_R_BRIGHT_PIN_PB1);
			sat4 &= ~(1 << HC595_4_TEXT_R_BRIGHT_PIN_5);
			sat1 = 0x00;
			sat2 = 0x00;
		}
	}
	
	soft_spi();
	
	anim_parameters[ANIM_TYPE_BRIGHTNESS_CHANGED].started_time_ms = get_system_ms();
	(*current_frame)++;
	
	if ((*current_frame) > 8) {
		anim_parameters[ANIM_TYPE_BRIGHTNESS_CHANGED].started = 0;
		brightness_change(brightness_mode, current.brightness[brightness_mode]);
	}
}

static void show_vu_anim()
{
	uint8_t *current_frame = &anim_parameters[ANIM_TYPE_SHOW_VU].current_frame;
	
	switch(*current_frame) {
		case 0:
			shoe_all_vu();
			(*current_frame)++;
			break;	
		case 1:
			if (!has_time_elapsed(anim_parameters[ANIM_TYPE_SHOW_VU].started_time_ms, 500)) {
				return;
			}
			(*current_frame)++;
			break;
		default:
			hide_all_vu();
			anim_parameters[ANIM_TYPE_SHOW_VU].started = 0;
			break;
	}
}

static void startup_anim()
{
	uint8_t anim_type = anim_parameters[ANIM_TYPE_STARTUP].parameters[0];
	uint8_t *current_frame = &anim_parameters[ANIM_TYPE_STARTUP].current_frame;
	uint16_t *started_time_ms = &anim_parameters[ANIM_TYPE_STARTUP].started_time_ms;
		
	switch(*current_frame) {
	
		// Запуск левой анимации	
		case 0:  
			sat1 = 0x00;
			sat2 = 0x00;
			soft_spi();
			start_anim(ANIM_SATURN_TEXT_LEFT, 1, anim_type);
			(*current_frame)++;
			break;
			
		// Ожидание
		case 1:  
			if (!has_time_elapsed((*started_time_ms), 600)) {
				return;
			}
			(*current_frame)++;
			break;
		
		// Запуск правой анимации
		case 2:  
			start_anim(ANIM_SATURN_TEXT_RIGHT, 1, anim_type);
			(*current_frame)++;
			break;
		
		// Ожидание
		case 3: 
			if (!has_time_elapsed((*started_time_ms), 1200)) {
				return;
			}
			(*current_frame)++;
		
			break;
		case 4:  
			anim_parameters[ANIM_TYPE_STARTUP].started = 0;
			anim_parameters[ANIM_TYPE_STARTUP].started_time_ms = 0;
			
			if (current.saturn_text_on == 1) {
				sat1 = 0xFF;
				sat2 = 0xFF;
				soft_spi();
			} else {
				sat1 = 0x00;
				sat2 = 0x00;
				soft_spi();
			}

			break;
	}
}

static void saturn_text_anim(uint8_t channel)
{	
	uint8_t anim_type;
	volatile uint8_t *sat;
	if (channel == LEFT_CHANNEL) {
		anim_type = ANIM_SATURN_TEXT_LEFT;
		sat = &sat1;
	} else {
		anim_type = ANIM_SATURN_TEXT_RIGHT;
		sat = &sat2;
	}

	if (!has_time_elapsed(anim_parameters[anim_type].started_time_ms, 60)) {
		return;
	}
	uint8_t dot_type = anim_parameters[anim_type].parameters[0];
	if (dot_type == TEXT_SATURN_ANIM_TYPE_DOT) {
		(*sat) = 0x00;
	}

	uint8_t *current_frame = &anim_parameters[anim_type].current_frame;

	if ((*current_frame) < 7) {
		uint8_t current_frame_ = 7 - (*current_frame);
		(*sat) = (*sat) | (1 << current_frame_);
		soft_spi();
		anim_parameters[anim_type].started_time_ms = get_system_ms();
		(*current_frame)++;
		return;
	}
	
	anim_parameters[anim_type].started = 0;
}
