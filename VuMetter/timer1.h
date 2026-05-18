/*
 * timer1.h
 *
 * Created: 24.01.2026 2:18:33
 *  Author: max4d
 */ 

#ifndef TIMER1_H_
#define TIMER1_H_

#include "main.h"

void timer1_init();
uint16_t get_system_ms();
uint16_t time_diff(uint16_t newer, uint16_t older);
uint8_t has_time_elapsed(uint16_t start_time, uint16_t timeout_ms);
uint8_t time_expired(uint16_t time);

#endif /* TIMER1_H_ */