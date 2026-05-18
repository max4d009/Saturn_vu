/*
 * config.h
 *
 * Created: 15.01.2026 2:34:53
 *  Author: max4d
 */ 

#ifndef CONFIG_H_
#define CONFIG_H_

#include "main.h"

void peaks_next_mode();

void peaks_next_time();
void brightness_next_mode();
void update_parameters();
void brightness_change(uint8_t brightness_mode, uint8_t brightness);
uint8_t get_peaks_mode();
uint8_t get_peaks_time_id();
void set_peaks_mode(uint8_t mode);
void set_peaks_time_id(uint8_t time_id);

#endif /* CONFIG_H_ */