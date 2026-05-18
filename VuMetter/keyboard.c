/*
 * keyboard.h
 *
 * Created: 15.01.2026 2:18:32
 *  Author: max4d
 */ 

#include "keyboard.h"

static uint32_t press_start_time = 0;
static uint8_t pressed_key = KEY_NONE;
static uint8_t keyboard_mode = KEYBOARD_MODE_DEFAULT;
static uint8_t long_press_handled = 0;

static void button_brightness_pressed(uint8_t long_pressed);
static void button_peak_mode_pressed(uint8_t long_pressed);
static void button_peak_time_pressed(uint8_t long_pressed);
static void button_brightness_change_up(uint8_t long_pressed);
static void button_brightness_change_down(uint8_t long_pressed);
static void eeprom_save();

void keyboard_init()
{
	pressed_key = KEY_NONE;
	
	buttons[KEYBOARD_MODE_DEFAULT][BUTTON_0].operation = button_brightness_pressed;
	buttons[KEYBOARD_MODE_DEFAULT][BUTTON_1].operation = button_peak_mode_pressed;
	buttons[KEYBOARD_MODE_DEFAULT][BUTTON_2].operation = button_peak_time_pressed;
	
	buttons[KEYBOARD_MODE_BRIGHTNES][BUTTON_0].operation = button_brightness_pressed;
	buttons[KEYBOARD_MODE_BRIGHTNES][BUTTON_1].operation = button_brightness_change_down;
	buttons[KEYBOARD_MODE_BRIGHTNES][BUTTON_2].operation = button_brightness_change_up;
}

uint8_t get_key(uint16_t adc)
{
    if (adc > 680 && adc < 800) return BUTTON_2;
    if (adc > 214 && adc < 254) return BUTTON_0;
    if (adc > 19 && adc < 50) return BUTTON_1;
    
    return KEY_NONE;
}

void keys_update()
{
	static uint32_t last_poll_time = 0;
	uint32_t now = get_system_ms();
	    
	// Опрашиваем кнопки не чаще чем раз в 10 мс
	if (now - last_poll_time < 10) {
		return;
	}
	last_poll_time = now;
	
	uint16_t adc = m4d_adc_get_keyboard();
	uint8_t key = get_key(adc);

	// Обработка нажатий
	if (key != KEY_NONE && pressed_key == KEY_NONE) {
		// Начало нажатия
		pressed_key = key;
		press_start_time = now;
	    long_press_handled = 0;
	}
	
    if (key == KEY_NONE && pressed_key != KEY_NONE) {
        if (!long_press_handled) { // Только если длинное нажатие не было обработано
            uint32_t press_duration = now - press_start_time;
            if (press_duration <= 1000) {
                buttons[keyboard_mode][pressed_key].operation(0);
            }
            // Для press_duration > 1000 ничего не делаем
        }
        pressed_key = KEY_NONE;
        return; // Важно!
    }
	
	// Если кнопка удерживается больше 1 сек
	if (key == pressed_key && key != KEY_NONE) {
		if (!long_press_handled && (now - press_start_time > 1000)) {
			buttons[keyboard_mode][pressed_key].operation(1);
			long_press_handled = 1;
		}
	}
}

static void button_brightness_pressed(uint8_t long_pressed)
{
	if (long_pressed == 1) {
		start_anim(ANIM_TYPE_STARTUP, 1, TEXT_SATURN_ANIM_TYPE_DOT);
		keyboard_mode = KEYBOARD_MODE_DEFAULT;
		return;
	}
	
	keyboard_mode = KEYBOARD_MODE_BRIGHTNES;
	changed_parameters.brightness_mode++;
	if (changed_parameters.brightness_mode >= BRIGHTNESS_CONFIG_COUNT) {
		changed_parameters.brightness_mode = BRIGHTNESS_BACKLIGHT_CONFIG;
	}
}

static void button_brightness_change_up(uint8_t long_pressed)
{
	if (long_pressed == 1) {
		eeprom_save();
		return;
	}
	
	if (current.brightness_mode == BRIGHTNESS_TEXT_CONFIG && current.saturn_text_on != 1) {
		changed_parameters.saturn_text_on = 1;
		return;
	}
	
	changed_parameters.brightness[current.brightness_mode] = 1;
}

static void button_brightness_change_down(uint8_t long_pressed)
{
	if (long_pressed == 1) {
		eeprom_save();
		return;
	}
	
	if (current.brightness_mode == BRIGHTNESS_TEXT_CONFIG && current.brightness[BRIGHTNESS_TEXT_CONFIG] == 0 && current.saturn_text_on == 1 ) {
		changed_parameters.saturn_text_on = 0;
		return;
	}
	
	changed_parameters.brightness[current.brightness_mode] = 0;
}

static void button_peak_mode_pressed(uint8_t long_pressed)
{
	if (long_pressed == 1) {
		save_peaks_mode();
		return;
	}
	
	peaks_next_mode();
}

static void button_peak_time_pressed(uint8_t long_pressed)
{	
	if (long_pressed == 1) {
		 save_peaks_time();
		return;
	}
	
	peaks_next_time();
}

static void eeprom_save()
{
	switch (current.brightness_mode) {
		case BRIGHTNESS_VU_CONFIG:
			EEPROM_write(PR_MODE_BRIGTNESS_VU, current.brightness[BRIGHTNESS_VU_CONFIG]);
			break;
		
		case BRIGHTNESS_TEXT_CONFIG:
			EEPROM_write(PR_MODE_BRIGTNESS_TEXT, current.brightness[BRIGHTNESS_TEXT_CONFIG]);
			EEPROM_write(PR_MODE_SATURN_TEXT_ENABLED, current.saturn_text_on);
			break;
		
		case BRIGHTNESS_BACKLIGHT_CONFIG:
			EEPROM_write(PR_MODE_BRIGTNESS_BACKLIGHT, current.brightness[BRIGHTNESS_BACKLIGHT_CONFIG]);
			break;
	}
	
	start_anim(ANIM_TYPE_STARTUP, 1, TEXT_SATURN_ANIM_TYPE_DOT);
}