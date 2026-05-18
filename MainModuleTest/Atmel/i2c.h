/*
 * i2c.h
 *
 *  Created: 17.12.2020 1:03:28
 *  Author: m4d
 */ 
#ifndef I2C_H_
#define I2C_H_

#include "main.h"

void i2c_init_as_master(void);
void i2c_receive_timer(uint8_t module_addr, uint8_t sla_w_module, uint8_t sla_r_module);
void i2c_send_vu_params(char transact_symbol, uint8_t value);
void send_from_query_timer();

#define VU_ADDR 0x3E
#define SLA_R_VU VU_ADDR|0b00000001 // Макрос адреса + бит чтения
#define SLA_W_VU VU_ADDR&0b11111110 // Макрос адреса + бит записи

#define I2C_VU_START_TRANSACTION_PEAKS_MODE 1
#define I2C_VU_START_TRANSACTION_PEAKS_MODE_SAVE 2
#define I2C_VU_START_TRANSACTION_PEAKS_TIME 3
#define I2C_VU_START_TRANSACTION_PEAKS_TIME_SAVE 4

// #define TW_MT_DATA_ASK 0x28 // Ведущий передал данные и ведомый подтвердил приём
#define TW_MR_DATA_ACK 0x50 // Ведущий принял данные и передал подтверждение
#define TW_MR_DATA_NACK 0x58 // Ведущий передал данные и ведомый подтвердил приём

#define I2C_TIMEOUT 10000

#define I2C_DATA_VU_PEAKS_MODE 0
#define I2C_DATA_VU_PEAKS_TIME 1
#define I2C_DATA_VU_COUNT 2

#define I2C_DATA_COUNT 5
#define I2C_DATA_ALL_COUNT 29

#define I2C_SEND_QUERY_COUNT 10

struct I2CReadByte {
	unsigned char byte;
	uint8_t error;
};

struct I2CReadByte i2c_byte_d;

struct Query {
	uint8_t i2c_data[6];
	uint8_t sended;
	uint8_t count;
};

struct Query query_list[I2C_SEND_QUERY_COUNT];

volatile uint8_t i2c_data[I2C_DATA_ALL_COUNT + 1];

#endif /* I2C_H_ */