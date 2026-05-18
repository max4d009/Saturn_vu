/*
 * anim.h
 *
 * Created: 23.01.2026 3:20:02
 *  Author: max4d
 */ 


#ifndef ANIM_H_
#define ANIM_H_

#include "main.h"

#define ANIM_TYPE_PEAKS_TIME_CHANGED 0
#define ANIM_TYPE_BRIGHTNESS_CHANGED 1
#define ANIM_TYPE_STARTUP 2
#define ANIM_SATURN_TEXT_LEFT 3
#define ANIM_SATURN_TEXT_RIGHT 4
#define ANIM_TYPE_SHOW_VU 5

#define ANIM_MAX_PARAMS 6

struct AnimParameters {
	uint16_t started_time_ms;
	uint8_t started;
	uint8_t current_frame;
	uint8_t parameters[ANIM_MAX_PARAMS];
};
struct AnimParameters anim_parameters[10];

void start_anim(uint8_t anim_type, uint8_t param_count, ...);
void anim_timer();

#endif /* ANIM_H_ */