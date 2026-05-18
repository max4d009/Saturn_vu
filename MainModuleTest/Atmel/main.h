/*
 * main.h
 *
 * ? Author: max4d
 */ 
#ifndef _MAIN_H
#define _MAIN_H

#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <compat/twi.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "i2c.h"
#include "buttons.h"
#include "led.h"

struct CurrentParameters {
	uint8_t vu_peaks_mode;
	uint8_t vu_peaks_time;
};
struct CurrentParameters current;

#endif