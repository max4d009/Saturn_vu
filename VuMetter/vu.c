/*
 * vu.c
 *
 * Created: 24.01.2026 2:06:05
 *  Author: max4d
 */ 
#include "vu.h"

static int8_t vu_array[2][VU_LEDS] = {0};
static int8_t prevDiodNum[2] = {-1, -1};
static uint16_t lastPeakTime[2] = {0, 0};

void shoe_all_vu()
{
		for (int8_t i = 0; i < VU_LEDS; i++) {
			vu_array[RIGHT_CHANNEL][i] = 1;
		}
		show_left_vu();
		show_right_vu();
}

void hide_all_vu()
{
	for (int8_t i = 0; i < 4; i++) { //1,2,3,4
		sat4 &= ~(1 << (i+1));
	}
	
	for (int8_t i = 4; i < 9; i++) { //5,6,7,8
		sat3 &= ~(1 << (i-4));
	}
	
	PORTC &= ~(1 << VU_20DB_MK_PIN_PC2);
	
	for (int8_t i = 1; i < 9; i++) {
		PORTD &= ~(1 << (i-1));
	}
	
	soft_spi();
	
	reset_vu_array();
}

void show_left_vu()
{
	if (vu_array[LEFT_CHANNEL][0] == 1) {
		PORTC |= (1 << VU_20DB_MK_PIN_PC2);
	} else {
		PORTC &= ~(1 << VU_20DB_MK_PIN_PC2);
	}
	
	for (int8_t i = 1; i < 9; i++) {
		if (vu_array[LEFT_CHANNEL][i] == 1) {
			PORTD |= (1 << (i-1));
		} else {
			PORTD &= ~(1 << (i-1));
		}
	}
}

void show_right_vu()
{
	for (int8_t i = 0; i < 4; i++) { //1,2,3,4
		if (vu_array[RIGHT_CHANNEL][i] == 1) {
			sat4 |= (1 << (i+1));
		} else {
			sat4 &= ~(1 << (i+1));
		}
	}
	
	for (int8_t i = 4; i < 9; i++) { //5,6,7,8
		if (vu_array[RIGHT_CHANNEL][i] == 1) {
			sat3 |= (1 << (i-4));
		} else {
			sat3 &= ~(1 << (i-4));
		}
	}
	
	soft_spi();
}

void reset_vu_array()
{
	show_left_vu();
	show_right_vu();
	
	uint16_t system_ms_ = get_system_ms();
	
	vu_update(0, LEFT_CHANNEL, system_ms_);
	vu_update(0, RIGHT_CHANNEL, system_ms_);
}

void vu_update(int8_t diodNum, uint8_t channel, uint16_t system_ms)
{
	for (int8_t i = 0; i < VU_LEDS; i++) {
		vu_array[channel][i] = 0;
	}
	
	if (current.peaks_mode == PEAKS_MODE_OFF) {
		show_left_vu();
		show_right_vu();
		return;
	}

	if (current.peaks_mode == PEAKS_MODE_SOLID) {
		for (int8_t i = 0; i < VU_LEDS; i++) {
			vu_array[channel][i] = 1;
		}
		show_left_vu();
		show_right_vu();
		return;
	}
	
	if (diodNum >= prevDiodNum[channel]) {
		if (current.peaks_mode == PEAKS_MODE_DOT) {
			vu_array[channel][diodNum] = 1;
		} else if (current.peaks_mode == PEAKS_MODE_BAR) {
			for (uint8_t i = 0; i <= diodNum; i++) {
				vu_array[channel][i] = 1;
			}
		}
		
		if (channel == LEFT_CHANNEL) {
			show_left_vu();
		} else {
			show_right_vu();
		}
		
		prevDiodNum[channel] = diodNum;
		lastPeakTime[channel] = system_ms;
	} else if (diodNum < prevDiodNum[channel]) {
		uint16_t now = system_ms;
		if (time_diff(now, lastPeakTime[channel]) < peaks_times_ms[current.peaks_time_id]) {
			return;
		}
		
		prevDiodNum[channel]--;
		if (prevDiodNum[channel] < -1) {
			prevDiodNum[channel] = -1;
		}
		
		if (prevDiodNum[channel] >= 0) {
			if (current.peaks_mode == PEAKS_MODE_DOT) {
				vu_array[channel][prevDiodNum[channel]] = 1;
			} else if (current.peaks_mode == PEAKS_MODE_BAR) {
				for (uint8_t i = 0; i <= prevDiodNum[channel]; i++) {
					vu_array[channel][i] = 1;
				}
			}
		}

		if (channel == LEFT_CHANNEL) {
			show_left_vu();
		} else {
			show_right_vu();
		}
		
		lastPeakTime[channel] = system_ms;
	}
}

const uint16_t peaks_times_ms[12] = {
	200,   // 1
	450,   // 2
	700,   // 3
	950,   // 4
	1200,  // 5
	1450,  // 6
	1700,  // 7
	1950,  // 8
	2200,  // 9
	2450,  // 10
	2700,  // 11
	2950   // 12
};