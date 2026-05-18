/*
 * main.c
 *
 * ? Author: max4d
 */ 
#include "main.h"

volatile uint8_t buttons_timer_flag = 0;
volatile uint8_t i2c_timer_flag = 0;

void timer1_init(void) {	
	TCCR1B |= (1 << WGM12);   // CTC режим
	TCCR1B |= (1 << CS11) | (1 << CS10);  // Предделитель 64
	OCR1A = 6249;              // 50 мс при 8 MHz
	TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
	static uint8_t counter_50ms = 0;
	
	// Сбрасываем флаг каждые 50 мс
	buttons_timer_flag = 1;
	counter_50ms++;
	
	// Каждые 20 раз по 50 мс = 1 секунда (20 * 50 мс = 1000 мс)
	if (counter_50ms >= 20) {
		counter_50ms = 0;
		i2c_timer_flag = 1;
	}
}

int main(void) {
	sei();
	
	current.vu_peaks_mode = 0;
	current.vu_peaks_time = 0;
	
	buttons_init();
	timer1_init();
	leds_init();
	
	while (1) {
		// Вызывается каждые 50 мс
		if (buttons_timer_flag) {
			buttons_timer_flag = 0;
			buttons_timer();
			send_from_query_timer(); // отправка данных по i2c
		}
		
		// Вызывается раз в секунду
		if (i2c_timer_flag) {
			i2c_timer_flag = 0;
			i2c_receive_timer(VU_ADDR, SLA_W_VU, SLA_R_VU);  // прием данных по i2c
		}
	}
}