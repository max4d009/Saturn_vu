/*
 * timer1.c
 *
 * Created: 24.01.2026 2:18:15
 *  Author: max4d
 */ 

#include "timer1.h"

// Системный тикер - обновляется каждую 1 мс
volatile uint16_t system_ms = 0;

void timer1_init()
{
	cli();
	
	// Полный сброс всех регистров таймера 1
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1C = 0;  // Если есть на ATmega328
	
	// Сброс счетчика
	TCNT1 = 0;
	
	// Установка OCR1A - когда прерывание
	OCR1A = 124;
		
	// 1. Режим CTC
	TCCR1B |= (1 << WGM12);
	
	// 2. Разрешить прерывание сравнения
	TIMSK1 |= (1 << OCIE1A);
	
	// 3. Сбросить флаг прерывания (на всякий случай)
	TIFR1 |= (1 << OCF1A);
	
	// 4. Включить предделитель 64
	TCCR1B |= (1 << CS11) | (1 << CS10);
	
	sei();
}

ISR(TIMER1_COMPA_vect)
{
	system_ms++;
}

uint16_t get_system_ms()
{
	uint16_t t;
	cli();
	t = system_ms;
	sei();
	return t;
}

uint16_t time_diff(uint16_t newer, uint16_t older)
{
	if (newer >= older) return newer - older;
	return (UINT16_MAX - older) + newer + 1;
}

uint8_t has_time_elapsed(uint16_t start_time, uint16_t timeout_ms)
{
	return time_diff(get_system_ms(), start_time) >= timeout_ms;
}

uint8_t time_expired(uint16_t time)
{
	return time_diff(get_system_ms(), time);
}