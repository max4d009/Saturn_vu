/*
 * vu.h
 *
 * Created: 24.01.2026 2:06:16
 *  Author: max4d
 */ 
#ifndef VU_H_
#define VU_H_


#include "main.h"

extern const uint16_t peaks_times_ms[12];

void vu_update(int8_t diodNum, uint8_t channel, uint16_t system_ms);
void reset_vu_array();
void shoe_all_vu();
void hide_all_vu();
void show_left_vu();
void show_right_vu();

#endif /* VU_H_ */