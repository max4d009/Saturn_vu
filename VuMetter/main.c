/*
 * vu.c
 *
 * λ Created: 11.07.2021 23:28:18
 * Author : m4d
 */ 
#include "main.h"

int main()
{
	EEPROM_init();
	
	DDRB |= (1 << VU_PEAK_R_BRIGHT_PIN_PB6);
	DDRB |= (1 << BACKLIGHT_R_BRIGHT_PIN_PB7);
	DDRB |= (1 << TEXT_R_BRIGHT_PIN_PB1);
	
	DDRB |= (1 << PLUS_MK_PIN_PB0);
	DDRB |= (1 << CLK) | (1 << DS) | (1 << E);
	DDRC |= (1 << MINUS_MK_PIN_PC3);
	DDRC |= (1 << VU_20DB_MK_PIN_PC2);
	DDRD = 0xFF;
	PORTD = 0x00;
	sat1 = sat2 = sat3 = sat4 = 0x00;

	soft_spi();
	timer1_init();
	
	init_leds();
	m4d_adc_init_8();
	i2c_init_as_slave();
	keyboard_init();

	current.brightness_mode = BRIGHTNESS_BACKLIGHT_CONFIG;
	
	current.brightness[BRIGHTNESS_BACKLIGHT_CONFIG] = EEPROM_read(PR_MODE_BRIGTNESS_BACKLIGHT);
	current.brightness[BRIGHTNESS_VU_CONFIG] = EEPROM_read(PR_MODE_BRIGTNESS_VU);
	current.brightness[BRIGHTNESS_TEXT_CONFIG] = EEPROM_read(PR_MODE_BRIGTNESS_TEXT);
	
	current.peaks_mode = EEPROM_read(PR_MODE_PEAKS_MODE);
	current.peaks_time_id = EEPROM_read(PR_MODE_PEAKS_TIME);
	current.saturn_text_on = EEPROM_read(PR_MODE_SATURN_TEXT_ENABLED);
	changed_parameters = current;
	
	_delay_ms(500);
	start_anim(ANIM_TYPE_STARTUP, 1, TEXT_SATURN_ANIM_TYPE_SOLID);
	
	brightness_change(BRIGHTNESS_BACKLIGHT_CONFIG, current.brightness[BRIGHTNESS_BACKLIGHT_CONFIG]);
	brightness_change(BRIGHTNESS_VU_CONFIG, current.brightness[BRIGHTNESS_VU_CONFIG]);
	brightness_change(BRIGHTNESS_TEXT_CONFIG, current.brightness[BRIGHTNESS_TEXT_CONFIG]);
   
    soft_spi();
    
	sei();

	reset_vu_array();
    while (1) 
    {				
		update_parameters();
		execute_command_timer();
		keys_update();
				
		uint16_t levelLeft = m4d_adc_get_level(LEFT_CHANNEL);
		uint16_t levelRight = m4d_adc_get_level(RIGHT_CHANNEL);
		int8_t numDiodLeft = get_vu_level(levelLeft);
		int8_t numDiodRight = get_vu_level(levelRight);

		additional_leds_timer(LEFT_CHANNEL, levelLeft);
		additional_leds_timer(RIGHT_CHANNEL, levelRight);

		uint16_t system_ms_ = get_system_ms();

		vu_update(numDiodLeft, LEFT_CHANNEL, system_ms_);
		vu_update(numDiodRight, RIGHT_CHANNEL, system_ms_);

		additional_led_timer();
		
		anim_timer();
	}
}
