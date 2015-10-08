/*
 * eeprom.h
 *
 *  Created on: 26.02.2012
 *      Author: Stefan
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#define EEPROM_CONFIG_ID	0
#define EEPROM_MOTION_ID	1

#define EEPROM_CONFIG_SIZE	0x10000
#define EEPROM_MOTION_SIZE	0x20000

unsigned char eeprom_init(void);
unsigned short eeprom_exists(char* name);
unsigned short eeprom_read(char* name, void* struc, unsigned short length);
unsigned short eeprom_write(char* name, void* struc, unsigned short length);
void eeprom_erase(unsigned char sure);
void eeprom_writeRaw(unsigned char id, unsigned int startreg, const unsigned char* data, unsigned short length);
void eeprom_readRaw(unsigned char id, unsigned int startreg, unsigned char* buffer, unsigned short length);

#endif /* EEPROM_H_ */
