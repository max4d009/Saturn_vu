/*
 * software_spi.c
 *
 * Created: 05.01.2026 18:39:37
 *  Author: max4d
 */ 

#include "software_spi.h"

volatile uint8_t sat1, sat2, sat3, sat4 = 0;
static void transmit(uint8_t data);

void soft_spi()
{
	uint8_t sat4_ = (sat4 & 0xF0) >> 4 | (sat4 & 0x0F) << 4;
	sat4_ = (sat4_ & 0xCC) >> 2 | (sat4_ & 0x33) << 2;
	sat4_ = (sat4_ & 0xAA) >> 1 | (sat4_ & 0x55) << 1;
	
	uint8_t sat3_ = (sat3 & 0xF0) >> 4 | (sat3 & 0x0F) << 4;
	sat3_ = (sat3_ & 0xCC) >> 2 | (sat3_ & 0x33) << 2;
	sat3_ = (sat3_ & 0xAA) >> 1 | (sat3_ & 0x55) << 1;
	
	PORTB &= ~(1<<E);
	
	transmit(sat4_);
	transmit(sat3_);
	transmit(sat2);
	transmit(sat1);
	
	PORTB |=  (1<<E);  // latch
	PORTB &= ~(1<<E);  // ready for next
}

static void transmit(uint8_t data)
{
	for (uint8_t k = 0; k < 8; k++) {
		PORTB &=~ (1 << CLK);
		_delay_us(2);
		
		if (data & (1 << k)) {
			PORTB |=  (1 << DS);
		} else {
			PORTB &= ~(1 << DS);
		}
		_delay_us(2);
		PORTB |= (1 << CLK);
		_delay_us(2);
	}
}
