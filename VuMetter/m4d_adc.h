/*
 * m4d_adc.h
 *
 * λ Created: 08.12.2020 1:18:27
 *  Author: m4d
 */ 
#ifndef M4D_ADC_H_
#define M4D_ADC_H_

#include "main.h"

uint16_t m4d_adc_get_keyboard();
int8_t get_vu_level(uint16_t adc);

void m4d_adc_init_8(void);
uint16_t m4d_adc_get_level(uint8_t channel);
struct adc get_adc_result();
void m4d_adc_reset_result();
#endif /* M4D_ADC_H_ */