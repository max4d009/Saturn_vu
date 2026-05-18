/*
 * led.c
 *
 *  Author: max4d
 */ 
#include "led.h"

void leds_init(void)
{
	// Настраиваем пины как выходы
	LED_MODE1_DDR |= (1 << LED_MODE1_PIN);
	LED_MODE2_DDR |= (1 << LED_MODE2_PIN);
	LED_MODE3_DDR |= (1 << LED_MODE3_PIN);
	LED_MODE4_DDR |= (1 << LED_MODE4_PIN);
	
	// Выключаем все светодиоды
	leds_all_off();
}

void leds_all_off(void)
{
	LED_MODE1_PORT &= ~(1 << LED_MODE1_PIN);
	LED_MODE2_PORT &= ~(1 << LED_MODE2_PIN);
	LED_MODE3_PORT &= ~(1 << LED_MODE3_PIN);
	LED_MODE4_PORT &= ~(1 << LED_MODE4_PIN);
}

void leds_set_mode(uint8_t mode)
{
	leds_all_off();
	
	switch(mode) {
		case 0:  // Режим 1
		LED_MODE1_PORT |= (1 << LED_MODE1_PIN);
		break;
		case 1:  // Режим 2
		LED_MODE2_PORT |= (1 << LED_MODE2_PIN);
		break;
		case 2:  // Режим 3
		LED_MODE3_PORT |= (1 << LED_MODE3_PIN);
		break;
		case 3:  // Режим 4
		LED_MODE4_PORT |= (1 << LED_MODE4_PIN);
		break;
		default:
		// Если режим не распознан, зажигаем все для индикации ошибки
		LED_MODE1_PORT |= (1 << LED_MODE1_PIN);
		LED_MODE2_PORT |= (1 << LED_MODE2_PIN);
		LED_MODE3_PORT |= (1 << LED_MODE3_PIN);
		LED_MODE4_PORT |= (1 << LED_MODE4_PIN);
		break;
	}
}

void leds_update(uint8_t peaks_mode)
{
	// Обновляем светодиоды в соответствии с текущим режимом
	leds_set_mode(peaks_mode);
}