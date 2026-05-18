/*
 * buttons.c
 *
 * ? Created: 23.10.2023 20:21:19
 *  Author: m4d
 */ 
#include "buttons.h"

static void select_peaks_mode();
static void select_peaks_time();

typedef struct {
	volatile uint8_t *DDR_REG;
	volatile uint8_t *PORT_REG;
	volatile uint8_t *PIN_REG;
	uint8_t pin;
	void (*operation)(void);
	uint8_t wait_period;
} ButtonConfig_t;

static const ButtonConfig_t button_cfg[NUM_BUTTONS] PROGMEM = {
	{ &DDRC, &PORTC, &PINC, PEAK_MODE_BUTTON_PIN, select_peaks_mode, 2},
	{ &DDRC, &PORTC, &PINC, TIME_PEAK_BUTTON_PIN, select_peaks_time, 2}
};

void buttons_timer()
{
	static uint8_t wait_count = 0;
	uint8_t any_key_pressed = 0;
	
	for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
		if (!(*button_list[i].PIN_REG & (1 << button_list[i].pin))) {
			any_key_pressed = 1;
			if (button_list[i].pressed == 1) {			
				continue;
			}

			if (wait_count < button_list[i].wait_period) {
				wait_count++;
			}
			
			if (wait_count == button_list[i].wait_period) {
				button_list[i].operation();
				button_list[i].pressed = 1;
				wait_count = 0;
			}
		} else {
			button_list[i].pressed = 0;
		}
	}
	if (any_key_pressed == 0) {
		wait_count = 0;
	}
}

void buttons_init()
{
    for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
	    ButtonConfig_t tmp;
	    memcpy_P(&tmp, &button_cfg[i], sizeof(ButtonConfig_t));

	    button_list[i].DDR_REG     = tmp.DDR_REG;
	    button_list[i].PORT_REG    = tmp.PORT_REG;
	    button_list[i].PIN_REG     = tmp.PIN_REG;
	    button_list[i].pin         = tmp.pin;
	    button_list[i].operation   = tmp.operation;
	    button_list[i].wait_period = tmp.wait_period;

	    *button_list[i].DDR_REG  &= ~(1 << button_list[i].pin);
	    *button_list[i].PORT_REG |=  (1 << button_list[i].pin);
    }
}


static void select_peaks_mode()
{	
	i2c_send_vu_params(I2C_VU_START_TRANSACTION_PEAKS_MODE, 3);
}

static void select_peaks_time()
{
	i2c_send_vu_params(I2C_VU_START_TRANSACTION_PEAKS_TIME, 3);
}