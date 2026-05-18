/*
 * eeprom.c
 *
 * λ Created: 19.08.2024 2:29:20
 *  Author: max4d
 */ 
#include "eeprom.h"

static void eeprom_init_if_needed(void);

void EEPROM_init()
{
	eeprom_init_if_needed();
}

void EEPROM_write(uint8_t uiAddress, unsigned char ucData)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE)) //ждем освобождения флага окончания последней операцией с памятью
	{}
	EEAR = uiAddress; //Устанавливаем адрес
	EEDR = ucData; //Пишем данные в регистр
	EECR |= (1<<EEMPE); //Разрешаем запись
	EECR |= (1<<EEPE); //Пишем байт в память
}

unsigned char EEPROM_read(uint8_t uiAddress)
{
	while(EECR & (1<<EEPE))
	{} //ждем освобождения флага окончания последней операцией с памятью
	EEAR = uiAddress; //Устанавливаем адрес
	EECR |= (1<<EERE); //Запускаем операцию считывания из памяти в регистр данных
	return EEDR; //Возвращаем результат
}

void save_peaks_mode()
{
	EEPROM_write(PR_MODE_PEAKS_MODE, current.peaks_mode);
	start_anim(ANIM_TYPE_STARTUP, 1, TEXT_SATURN_ANIM_TYPE_DOT);
}

void save_peaks_time()
{
	EEPROM_write(PR_MODE_PEAKS_TIME, current.peaks_time_id);
	start_anim(ANIM_TYPE_STARTUP, 1, TEXT_SATURN_ANIM_TYPE_DOT);
}

static void eeprom_init_if_needed(void)
{
	uint8_t flag0 = EEPROM_read(EEPROM_INIT_FLAG_0);
	uint8_t flag1 = EEPROM_read(EEPROM_INIT_FLAG_1);

	// Если флаг не установлен - первое включение после прошивки
	if (flag0 != INIT_FLAG_VALUE_0 || flag1 != INIT_FLAG_VALUE_1) {
		// Записываем значения по умолчанию
		EEPROM_write(PR_MODE_PEAKS_MODE, PEAKS_MODE_DOT);
		EEPROM_write(PR_MODE_PEAKS_TIME, 0);
		
		// Устанавливаем флаг инициализации
		EEPROM_write(EEPROM_INIT_FLAG_0, INIT_FLAG_VALUE_0);
		EEPROM_write(EEPROM_INIT_FLAG_1, INIT_FLAG_VALUE_1);
	}
}