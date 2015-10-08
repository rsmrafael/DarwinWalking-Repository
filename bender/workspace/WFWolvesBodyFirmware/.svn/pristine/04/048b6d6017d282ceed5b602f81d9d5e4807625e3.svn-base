/*
 * config.c
 *
 *  Created on: 14.04.2011
 *      Author: Stefan
 *
 */

#include "config.h"
#include "include/Board.h"
#include "dbgu/dbgu.h"
#include "eeprom/eeprom.h"

#define CONFIG_COUNT	4
#include "config_default.h"
#include "config_jonny.h"
#include "config_locutus.h"
#include "config_hugh.h"
#include "config_copper.h"

#define CONFIG_BIT_0   AT91C_PIO_PB5
#define CONFIG_BIT_1   AT91C_PIO_PB6

#define CURRENT_VERSION	0x01

unsigned char config_eeprom;
unsigned char config_id;

signed char config_servos_zeropos[CONFIG_COUNT][CONFIG_SERVOS_COUNT] = {
		CONFIG_SERVOS_ZEROPOS_0,
		CONFIG_SERVOS_ZEROPOS_1,
		CONFIG_SERVOS_ZEROPOS_2,
		CONFIG_SERVOS_ZEROPOS_3
};

struct t_config_servo_offsets {
	unsigned char version;
	signed char offset[32];
} config_servo_offsets;

void Config_Init(void) {
#ifndef DEBUG_X86
	config_eeprom = eeprom_init();

	if (config_eeprom == 1) {
		config_servo_offsets.version = CURRENT_VERSION;
		for (int i = 0; i < sizeof(config_servo_offsets.offset); i++) {
			config_servo_offsets.offset[i] = 0;
		}
		eeprom_read("SERVO OFFSETS", &config_servo_offsets, sizeof(config_servo_offsets));
	}

	AT91C_BASE_PIOB->PIO_ODR = CONFIG_BIT_0;		// set to input
	AT91C_BASE_PIOB->PIO_PER = CONFIG_BIT_0;		// set to PIO mode
	AT91C_BASE_PIOB->PIO_PPUER = CONFIG_BIT_0;		// enable pull up

	AT91C_BASE_PIOB->PIO_ODR = CONFIG_BIT_1;		// set to input
	AT91C_BASE_PIOB->PIO_PER = CONFIG_BIT_1;		// set to PIO mode
	AT91C_BASE_PIOB->PIO_PPUER = CONFIG_BIT_1;		// enable pull up

	unsigned char bit0 = (((AT91C_BASE_PIOB->PIO_PDSR) & CONFIG_BIT_0) > 0);
	unsigned char bit1 = (((AT91C_BASE_PIOB->PIO_PDSR) & CONFIG_BIT_1) > 0);
	config_id = (bit1 << 1) | (bit0);
#else
	config_id = 0;
#endif

	if (config_eeprom != 1) {
		char* name = "DEFAULT";
		switch (config_id) {
			case 0:
				name = CONFIG_NAME_0; break;
			case 1:
				name = CONFIG_NAME_1; break;
			case 2:
				name = CONFIG_NAME_2; break;
			case 3:
				name = CONFIG_NAME_3; break;
		}
		dbgu_printf("[Config] Using configuration %d -> %s\r\n", config_id, name);
	}
}

unsigned char Config_UseServoOffsets(void) {
	return CONFIG_SERVOS_USE_OFFSETS;
}

unsigned char Config_GetServoCount(void) {
	return CONFIG_SERVOS_COUNT;
}

signed char Config_GetServoOffset(unsigned char id) {
	if (config_eeprom == 1) {
		return config_servo_offsets.offset[id];
	} else {
		return config_servos_zeropos[config_id][id];
	}
}

void Config_SetServoOffset(unsigned char id, signed char offset) {
	if (config_eeprom == 1) {
		config_servo_offsets.offset[id] = offset;
	}
}

unsigned char Config_Save() {
	if (config_eeprom == 1) {
		signed short addr = eeprom_write("SERVO OFFSETS", &config_servo_offsets, sizeof(config_servo_offsets));
		if (addr > 0) {
			return 1;
		}
	}
	return 0;
}
