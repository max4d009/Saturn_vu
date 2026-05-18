/*
 * led.h
 *
 *  Author: max4d
 */ 

#ifndef LED_H_
#define LED_H_

#include "main.h"

#define LED_MODE1_PORT  PORTB
#define LED_MODE1_DDR   DDRB
#define LED_MODE1_PIN   PB0

#define LED_MODE2_PORT  PORTB
#define LED_MODE2_DDR   DDRB
#define LED_MODE2_PIN   PB1

#define LED_MODE3_PORT  PORTB
#define LED_MODE3_DDR   DDRB
#define LED_MODE3_PIN   PB2

#define LED_MODE4_PORT  PORTB
#define LED_MODE4_DDR   DDRB
#define LED_MODE4_PIN   PB3

void leds_init(void);
void leds_update(uint8_t peaks_mode);
void leds_all_off(void);
void leds_set_mode(uint8_t mode);



#endif /* LED_H_ */