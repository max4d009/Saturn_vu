/*
 * i2c.c
 *
 *  Created: 17.12.2020 0:42:18
 *  Author: m4d
 */ 
#include "i2c.h"

static uint8_t i2c_start_condition();
static uint8_t i2c_stop_condition();
static uint8_t i2c_send_data(uint8_t *data, uint8_t count);
static uint8_t i2c_send_byte(uint8_t data);
static void after_i2c_read(uint8_t module_addr);
static int8_t get_query_id();
static void i2c_send(uint8_t module, uint8_t type, uint8_t count_params,  ...);

static struct I2CReadByte i2c_read_byte_from_slave(uint8_t is_last);

static struct I2CReadByte byte_dto;

void send_from_query_timer()
{
	for (uint8_t i = 0; i < I2C_SEND_QUERY_COUNT; i++) {
		if (query_list[i].sended == 0) {
			i2c_send_data(query_list[i].i2c_data, query_list[i].count);
			query_list[i].sended = 1;
		}
	}
}

void i2c_init_as_master()
{
	for (uint8_t i = 0; i < I2C_SEND_QUERY_COUNT; i++) {
		query_list[i].sended = 1;
	}
	
	TWBR = 0x20;
 	TWCR |= (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
}	


static uint8_t i2c_communication_attempt(uint8_t module_addr, uint8_t sla_w_module, uint8_t sla_r_module, uint8_t send_count)
{
	// 1. ÏÅÐÂÛÉ START CONDITION
	i2c_start_condition();
	_delay_us(20); // Ïàóçà ïîñëå START
	
	// 2. ÎÒÏÐÀÂÊÀ ÀÄÐÅÑÀ ÄËß ÇÀÏÈÑÈ (SLA+W)
	i2c_send_byte(sla_w_module);
	_delay_us(50); // ÂÀÆÍÀß ÏÀÓÇÀ - äàåò âðåìÿ ñëåéâó ïîäãîòîâèòüñÿ
	
	// 3. ÏÎÂÒÎÐÍÛÉ START CONDITION
	i2c_start_condition();
	_delay_us(20); // Ïàóçà ïîñëå ïîâòîðíîãî START
	
	// 4. ÎÒÏÐÀÂÊÀ ÀÄÐÅÑÀ ÄËß ×ÒÅÍÈß (SLA+R)
	i2c_send_byte(sla_r_module);
	_delay_us(20); // Ïàóçà ïåðåä ÷òåíèåì äàííûõ
	
	
	// 5. ×ÒÅÍÈÅ ÄÀÍÍÛÕ ÎÒ ÑËÅÉÂÀ
	for (uint8_t i = 0; i < send_count - 1; i++) {
		byte_dto = i2c_read_byte_from_slave(0); // ×òåíèå ñ ACK
		i2c_data[i] = byte_dto.byte;
		_delay_us(10); // Êîðîòêàÿ ïàóçà ìåæäó áàéòàìè
	}
	
	// 6. ×ÒÅÍÈÅ ÏÎÑËÅÄÍÅÃÎ ÁÀÉÒÀ (ñ NACK)
	byte_dto = i2c_read_byte_from_slave(1); // ×òåíèå ñ NACK
	_delay_us(10);
	
	i2c_data[send_count - 1] = byte_dto.byte;
	
	// 7. STOP CONDITION
	i2c_stop_condition();
	_delay_us(10); // Ïàóçà ïîñëå STOP
	
	return 1; // ÓÑÏÅÕ!
}

void i2c_receive_timer(uint8_t module_addr, uint8_t sla_w_module, uint8_t sla_r_module)
{
	uint8_t send_count = 0;
	
    if (module_addr == VU_ADDR) {
		send_count = I2C_DATA_VU_COUNT;
	}
		
	i2c_communication_attempt(module_addr, sla_w_module, sla_r_module, send_count);
		   
	after_i2c_read(module_addr);
}


static struct I2CReadByte i2c_read_byte_from_slave(uint8_t is_last)
{
	struct I2CReadByte i2c_byte_dto = {0, 0};
	
	if (is_last == 0) {
		TWCR = ((1<<TWINT)|(1<<TWEN)|(1<<TWEA));  // ACK
	} else {
		TWCR = ((1<<TWINT)|(1<<TWEN));  // NACK
	}
	
    uint16_t i2c_wait = I2C_TIMEOUT;
    while (!(TWCR & (1<<TWINT))) {
	    if (--i2c_wait == 0) {
		    i2c_byte_dto.error = 1;
		    TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO);
		    return i2c_byte_dto;
	    }
	    _delay_us(1);
    }
	
	if (is_last == 0) {
		if ((TWSR & 0xF8) != TW_MR_DATA_ACK) { 
			i2c_byte_dto.error = 2;
			i2c_byte_dto.byte = 0;			
			TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO);
			return i2c_byte_dto;
		}
	} else {
		if ((TWSR & 0xF8) != TW_MR_DATA_NACK) {
			i2c_byte_dto.error = 3;
			i2c_byte_dto.byte = 0;
			TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO);
			return i2c_byte_dto;
		}
	}
	
	i2c_byte_dto.error = 0;
	i2c_byte_dto.byte = TWDR;
	
	_delay_us(50);
	return i2c_byte_dto;
}

static void after_i2c_read(uint8_t module_addr)
{
	if (module_addr == VU_ADDR) {
		current.vu_peaks_mode = i2c_data[I2C_DATA_VU_PEAKS_MODE];
		current.vu_peaks_time = i2c_data[I2C_DATA_VU_PEAKS_TIME];
		
		leds_update(current.vu_peaks_mode);
	}
}

static int8_t get_query_id()
{
	for (uint8_t i = 0; i < I2C_SEND_QUERY_COUNT; i++) {
		if (query_list[i].sended == 1) {
			return i;
		}
	}
	
	return -1;
}

static void i2c_send(uint8_t module, uint8_t type, uint8_t count_params,  ...)
{
	
	int8_t query_id = get_query_id();	
	
	if (query_id == -1) {
		return;
	}

	query_list[query_id].i2c_data[0] = module;
	query_list[query_id].i2c_data[1] = type;
	query_list[query_id].count = count_params+2;
	
	va_list param;
	va_start(param, count_params);
	for (uint8_t i = 0; i < count_params; i++) {
		query_list[query_id].i2c_data[i+2] = va_arg(param, int);		
	}
	va_end(param);
	query_list[query_id].sended = 0;
}

// value ñåé÷àñ íå èñïîëüçóåòñÿ. Ïðè ïîñòóïëåíèè êîìàíäû, ìîäóëü èíäèêàòîðîâ ïðîñòî âêëþ÷àåò ñëåäóþùèé ðåùèì.
// Íî â áóäóùåì ìîæíî áóäåò âêëþ÷èòü êîíêðåòíûé.
void i2c_send_vu_params(char transact_symbol, uint8_t value)
{	
	i2c_send(SLA_W_VU, transact_symbol, 1, value);
}

static uint8_t i2c_send_data(uint8_t *data, uint8_t count)
{
	_delay_us(30);
	i2c_start_condition();
	
	_delay_us(30);

	uint8_t i = 0;
	do {
		i2c_send_byte(data[i]);
		_delay_us(20);
		i++;
	} while (i < count);
	
	i2c_stop_condition();
	
	_delay_us(50);
	
	return 1;
}

static uint8_t i2c_start_condition()
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    uint16_t timeout = I2C_TIMEOUT;
    while (!(TWCR & (1<<TWINT)) && timeout--) {
	    _delay_us(1);
    }
	
    return (timeout > 0) ? 1 : 0;
}

static uint8_t i2c_send_byte(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	
    uint16_t timeout = I2C_TIMEOUT;
    while (!(TWCR & (1<<TWINT)) && timeout--) {
	    _delay_us(1);
    }
	
	if (timeout == 0) return 0;
    
    return 1;
}

static uint8_t i2c_stop_condition(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);

    uint16_t timeout = I2C_TIMEOUT;
    while ((TWCR & (1<<TWSTO)) && timeout--) {
	    _delay_us(1);
    }
	
	_delay_us(10);
    
    return (timeout > 0) ? 1 : 0;
}