/*
 * m4d_adc.с
 *
 * λ Created: 02.12.2020 1:24:46
 *  Author: m4d
 */ 

#include "m4d_adc.h"

static uint16_t adc_read();
static uint16_t median_of_three(uint16_t a, uint16_t b, uint16_t c);


void m4d_adc_init_8()
{
    ADMUX = (1 << REFS0);  // AVCC reference
	ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
}


int8_t get_vu_level(uint16_t adc)
{
	if (adc > 830) { // +3
		return 8;
	} else if (adc > 600) { // 0
		return 7;
	} else if (adc > 531) { // -1
		return 6;
	} else if (adc > 470) { // -2
		return 5;
	} else if (adc > 420) { // -3
		return 4;
	} else if (adc > 330) { // -5
		return 3;
	} else if (adc > 255) { // -7
		return 2;
	} else if (adc > 175) { // -10
		return 1;
	} else if (adc > 40) { // -20
		return 0;
	}
	return -1;
}

uint16_t m4d_adc_get_keyboard()
{
	// Выбираем канал ADC7
    ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3)); // Сбрасываем MUX[3:0]
    ADMUX |= (1 << MUX0) | (1 << MUX1) | (1 << MUX2);                    // Устанавливаем MUX[3:0]
	
	adc_read();

	// Считываем три значения и возвращаем медиану
	uint16_t a = adc_read();
	uint16_t b = adc_read();
	uint16_t c = adc_read();

	return median_of_three(a, b, c);
}

uint16_t m4d_adc_get_level(uint8_t channel)
{
	ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3)); // Сбрасываем MUX[3:0]
	if(channel == LEFT_CHANNEL){
		//ADMUX &= ~(1 << MUX0);
	} else {
		ADMUX |= (1 << MUX0);
	}
	
	adc_read();
	
	uint16_t a = adc_read();
	uint16_t b = adc_read();
	uint16_t c = adc_read();
	
	return median_of_three(a, b, c);
}

// Считать показания с ADC
static uint16_t adc_read()
{
	// Включаем АЦП
	ADCSRA |= (1 << ADSC);
	// Производим измерение
	while(ADCSRA & (1 << ADSC));
	return ADC;	
}

static uint16_t median_of_three(uint16_t a, uint16_t b, uint16_t c)
{
	if (a > b) { uint16_t t = a; a = b; b = t; }
	if (b > c) { uint16_t t = b; b = c; c = t; }
	if (a > b) { uint16_t t = a; a = b; b = t; }
		
	return b;
}