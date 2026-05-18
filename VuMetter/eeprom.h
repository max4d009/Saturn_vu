/*
 * eeprom.h
 *
 * λ Created: 19.08.2024 2:29:31
 *  Author: max4d
 */ 
#ifndef EEPROM_H_
#define EEPROM_H_

#include "main.h"

#define EEPROM_INIT_FLAG_0 5
#define EEPROM_INIT_FLAG_1 100
#define INIT_FLAG_VALUE_0 44
#define INIT_FLAG_VALUE_1 55

#define PR_MODE_PEAKS_MODE 10
#define PR_MODE_PEAKS_TIME 20
#define PR_MODE_BRIGTNESS_VU 30
#define PR_MODE_BRIGTNESS_TEXT 40
#define PR_MODE_BRIGTNESS_BACKLIGHT 50
#define PR_MODE_SATURN_TEXT_ENABLED 60

void EEPROM_init();
void EEPROM_write(uint8_t uiAddress, unsigned char ucData);
unsigned char EEPROM_read(uint8_t uiAddress);
void save_peaks_mode();
void save_peaks_time();

#endif /* EEPROM_H_ */