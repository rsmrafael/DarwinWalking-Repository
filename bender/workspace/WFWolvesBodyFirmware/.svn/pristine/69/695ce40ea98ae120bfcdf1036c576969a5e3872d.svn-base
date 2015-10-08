/*
 * eeprom.c
 *
 *  Created on: 26.02.2012
 *      Author: Stefan
 */

#include "tc/tc.h"
#include "i2c/i2c.h"
#include "dbgu/dbgu.h"
#include <string.h>

#define EEPROM_CONFIG_ADDR		0x50
#define EEPROM_CONFIG_BASE		0x10
#define EEPROM_CONFIG_END		0xffff
#define EEPROM_CONFIG_PAGESIZE	128

#define EEPROM_MOTION_ADDR_L	0x56
#define EEPROM_MOTION_PAGESIZE	256

#define EEPROM_WRITE_DUR	6 // ms

#define CONFIG_SIG_0	0xc0
#define CONFIG_SIG_1	0xf1
#define CONFIG_SIG_2	0xda
#define CONFIG_SIG_3	0x1a

#define MAX_NAME_LENGTH	16

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

unsigned char eeprom_found = 0;

void eeprom_writeBytes(unsigned char eeprom_addr, unsigned short pagesize, unsigned int startreg, const unsigned char* data, unsigned short length) {
	unsigned int lower_boundary = (startreg / pagesize) * pagesize;
	unsigned int upper_boundary = lower_boundary + pagesize;

	if ((startreg + length) - lower_boundary > pagesize) {
		//dbgu_printf("[EEPROM] writeBytes: Writing %d bytes from 0x%x crosses page boundary at 0x%x\r\n", length, startreg, upper_boundary);
		//dbgu_printf("[EEPROM] writeBytes: Writing %d bytes at 0x%x from 0x%x\r\n", upper_boundary - startreg, startreg, data);
		if (startreg >= 0x10000) {
			I2C_WriteMultipleRegisters2(eeprom_addr | 1, startreg - 0x10000, data, upper_boundary - startreg);
		} else {
			I2C_WriteMultipleRegisters2(eeprom_addr, startreg, data, upper_boundary - startreg);
		}
		TC_DelayMs(EEPROM_WRITE_DUR);
		data += (upper_boundary - startreg);
		unsigned short remaining = length - (upper_boundary - startreg);
		unsigned int addr = upper_boundary;
		while (remaining > 0) {
			if (remaining <= pagesize) {
				//dbgu_printf("[EEPROM] writeBytes: Writing %d bytes at 0x%x from 0x%x\r\n", remaining, addr, data);
				if (addr >= 0x10000) {
					I2C_WriteMultipleRegisters2(eeprom_addr | 1, addr - 0x10000, data, remaining);
				} else {
					I2C_WriteMultipleRegisters2(eeprom_addr, addr, data, remaining);
				}
				remaining = 0;
			} else {
				//dbgu_printf("[EEPROM] writeBytes: Writing %d bytes at 0x%x from 0x%x\r\n", EEPROM_PAGESIZE, addr, data);
				if (addr >= 0x10000) {
					I2C_WriteMultipleRegisters2(eeprom_addr | 1, addr - 0x10000, data, pagesize);
				} else {
					I2C_WriteMultipleRegisters2(eeprom_addr, addr, data, pagesize);
				}
				remaining -= pagesize;
				data += pagesize;
			}
			TC_DelayMs(EEPROM_WRITE_DUR);
		}
	} else {
		if (startreg >= 0x10000) {
			I2C_WriteMultipleRegisters2(eeprom_addr | 1, startreg - 0x10000, data, length);
		} else {
			I2C_WriteMultipleRegisters2(eeprom_addr, startreg, data, length);
		}
		TC_DelayMs(EEPROM_WRITE_DUR);
	}
}

unsigned char eeprom_init() {
	unsigned char data[4];
	I2C_ReadMultipleRegisters2(EEPROM_CONFIG_ADDR, 0, &data[0], 4);

	//dbgu_printf("[EEPROM] Config ID: %x-%x-%x-%x\r\n", data[0], data[1], data [2], data[3]);
	if (data[0] != CONFIG_SIG_0 || data[1] != CONFIG_SIG_1 || data[2] != CONFIG_SIG_2 || data[3] != CONFIG_SIG_3) {
		data[0] = CONFIG_SIG_0; data[1] = CONFIG_SIG_1; data[2] = CONFIG_SIG_2; data[3] = CONFIG_SIG_3;
		eeprom_writeBytes(EEPROM_CONFIG_ADDR, EEPROM_CONFIG_PAGESIZE, 0, &data[0], 4);
		data[0] = 0x00; data[1] = 0x00; data[2] = 0x00; data[3] = 0x00;
		I2C_ReadMultipleRegisters2(EEPROM_CONFIG_ADDR, 0, &data[0], 4);
		if (data[0] == CONFIG_SIG_0 && data[1] == CONFIG_SIG_1 && data[2] == CONFIG_SIG_2 && data[3] == CONFIG_SIG_3) {
			dbgu_printf("[EEPROM] Found uninitialized EEPROM!\r\n");
			eeprom_found = 1;
		} else {
			dbgu_printf("[EEPROM] No EEPROM found!\r\n");
			eeprom_found = 0;
		}
	} else {
		dbgu_printf("[EEPROM] Found EEPROM!\r\n");
		eeprom_found = 1;
	}
	return eeprom_found;
}

unsigned short eeprom_access(char* name, void* struc, unsigned short length, unsigned char alwaysWrite) {
	unsigned short addr = EEPROM_CONFIG_BASE;

	while (addr < EEPROM_CONFIG_END) {
		//dbgu_printf("[EEPROM] access: Checking address 0x%x\r\n", addr);

		unsigned short curAddr = addr;
	    // Get length of the data field
	    unsigned short dataLen;
	    I2C_ReadMultipleRegisters2(EEPROM_CONFIG_ADDR, curAddr, (unsigned char*)&dataLen, sizeof(dataLen));
	    curAddr += sizeof(dataLen);

	    // Has the data length ever been written?  If so, we should be able to
	    // find a record here.
	    if (dataLen != 0xffff) {
	        // Get record name and compare with the desired name
			unsigned char match = 1;
			char c;
			const char* n = name;
			for (unsigned char i = 0; i < MAX_NAME_LENGTH; i++) {
				I2C_ReadMultipleRegisters2(EEPROM_CONFIG_ADDR, curAddr, (unsigned char*)&c, 1);
				//dbgu_printf("[EEPROM] access: Read character %c (0x%x) from address 0x%x\r\n", c, c, curAddr);
				curAddr++;
				if (c != *n++) {
					match = 0;
				}
				if (c == '\0') {
					break;
				}
			}

		    // Header checksum valid?
			unsigned char checksum;
		    I2C_ReadMultipleRegisters2(EEPROM_CONFIG_ADDR, curAddr, &checksum, 1);
		    curAddr++;
			if (1 == 1) {
				// Is this the desired record?  Read the data field once to test its
				// checksum.

				if (match == 1) {
					dbgu_printf("[EEPROM] access: Found entry '%s' at address 0x%x\r\n", name, addr);
					/*
					NvField pay(eeprom, hdr.next());
					uint8_t t;
					for (size_t i = 0; i < len; i++)
						pay.read(t);
					*/
					// If data field checksum is good, copy out data field to variable,
					// else fall back to supplied default values.
				    I2C_ReadMultipleRegisters2(EEPROM_CONFIG_ADDR, curAddr + length, &checksum, 1);
					if (1 == 1 && !(alwaysWrite == 1)) {
						I2C_ReadMultipleRegisters2(EEPROM_CONFIG_ADDR, curAddr, (unsigned char*)struc, length);
					} else {
						if (alwaysWrite != 1) {
							dbgu_printf("[EEPROM] Checksum mismatch, using defaul values!\r\n");
						}
						eeprom_writeBytes(EEPROM_CONFIG_ADDR, EEPROM_CONFIG_PAGESIZE, curAddr, (unsigned char*)struc, length);
						unsigned char temp = 0xc0; // Would be checksum
						eeprom_writeBytes(EEPROM_CONFIG_ADDR, EEPROM_CONFIG_PAGESIZE, curAddr + length, &temp, 1);
					}
					// In either case, we're done.
					return curAddr;
				} else {
					// Not the desired record; continue searching.
					addr = curAddr + dataLen + 1; // 1 for checksum field
					continue;
				}
			}
	    }
	    // No record ever written here, or can't read header -- stop searching.
	    break;
	}
	dbgu_printf("[EEPROM] access: Entry '%s' not found!\r\n", name);

	// Check that a new record will fit within our part of the device.
	unsigned short recLen = sizeof(unsigned short) // payload length
			+ MIN(strlen(name) + 1, MAX_NAME_LENGTH) // record name
			+ 1 // header checksum
			+ length // payload length
			+ 1; // payload checksum

	if (addr >= EEPROM_CONFIG_END - recLen) {
		dbgu_printf("[EEPROM] access: No space left for record of length 0x%x\r\n", recLen);
		return 0;
	}

	// Create a new record and initialize its data field to the supplied
	// default values.
	dbgu_printf("[EEPROM] access: Writing new entry at address 0x%x\r\n", addr);
	eeprom_writeBytes(EEPROM_CONFIG_ADDR, EEPROM_CONFIG_PAGESIZE, addr, (unsigned char*)&length, 2);
	addr += 2;
	//dbgu_printf("[EEPROM] access: Writing name of length %d at 0x%x\r\n", MIN(strlen(name), MAX_NAME_LENGTH - 1), addr);
	eeprom_writeBytes(EEPROM_CONFIG_ADDR, EEPROM_CONFIG_PAGESIZE, addr, (unsigned char*)name, MIN(strlen(name), MAX_NAME_LENGTH - 1));
	addr += MIN(strlen(name), MAX_NAME_LENGTH - 1);
	unsigned char temp = '\0';
	//dbgu_printf("[EEPROM] access: Writing 0 at address 0x%x\r\n", addr);
	eeprom_writeBytes(EEPROM_CONFIG_ADDR, EEPROM_CONFIG_PAGESIZE, addr, &temp, 1);
	addr++;
	temp = 0xc0; // Would be checksum
	//dbgu_printf("[EEPROM] access: Writing checksum at address 0x%x\r\n", addr);
	eeprom_writeBytes(EEPROM_CONFIG_ADDR, EEPROM_CONFIG_PAGESIZE, addr, &temp, 1);
	addr++;

	//dbgu_printf("[EEPROM] access: Writing data of length %d at address 0x%x\r\n", length, addr);
	eeprom_writeBytes(EEPROM_CONFIG_ADDR, EEPROM_CONFIG_PAGESIZE, addr, (unsigned char*)struc, length);
	temp = 0xc0; // Would be checksum
	//dbgu_printf("[EEPROM] access: Writing checksum2 at address 0x%x\r\n", addr+length);
	eeprom_writeBytes(EEPROM_CONFIG_ADDR, EEPROM_CONFIG_PAGESIZE, addr + length, &temp, 1);

	return addr;
}

unsigned short eeprom_exists(char* name) {
	unsigned short addr = EEPROM_CONFIG_BASE;

	while (addr < EEPROM_CONFIG_END) {
		unsigned short curAddr = addr;
	    // Get length of the data field
	    unsigned short dataLen;
	    I2C_ReadMultipleRegisters2(EEPROM_CONFIG_ADDR, curAddr, (unsigned char*)&dataLen, sizeof(dataLen));
	    curAddr += sizeof(dataLen);

	    // Has the data length ever been written?  If so, we should be able to
	    // find a record here.
	    if (dataLen != 0xffff) {
	        // Get record name and compare with the desired name
			unsigned char match = 1;
			char c;
			const char* n = name;
			for (unsigned char i = 0; i < MAX_NAME_LENGTH; i++) {
				I2C_ReadMultipleRegisters2(EEPROM_CONFIG_ADDR, curAddr, (unsigned char*)&c, 1);
				//dbgu_printf("[EEPROM] access: Read character %c (0x%x) from address 0x%x\r\n", c, c, curAddr);
				curAddr++;
				if (c == '\0') {
					break;
				}
				if (c != *n++) {
					match = 0;
				}
			}

		    // Header checksum valid?
			unsigned char checksum;
		    I2C_ReadMultipleRegisters2(EEPROM_CONFIG_ADDR, curAddr, &checksum, 1);
		    curAddr++;
			if (1 == 1) {
				// Is this the desired record?  Read the data field once to test its
				// checksum.
				if (match == 1) {
					return 1;
				} else {
					// Not the desired record; continue searching.
					addr = curAddr + dataLen + 1; // 1 for checksum field
					continue;
				}
			}
	    }
	    // No record ever written here, or can't read header -- stop searching.
	    break;
	}
	return 0;
}

unsigned short eeprom_read(char* name, void* struc, unsigned short length) {
	if (eeprom_found == 1) {
		return eeprom_access(name, struc, length, 0);
	}
	return 0;
}

unsigned short eeprom_write(char* name, void* struc, unsigned short length) {
	return eeprom_access(name, struc, length, 1);
}

void eeprom_erase(unsigned char sure) {
	if (sure == 1) {
		unsigned short length = EEPROM_CONFIG_END - 0;
		dbgu_printf("[EEPROM] Clearing %d bytes of EEPROM... ", length);
		unsigned char tmp[64];
		for (int i = 0; i < sizeof(tmp); i++) {
			tmp[i] = 0xff;
		}
		for (int i = 0; i < length / 64; i++) {
			eeprom_writeBytes(EEPROM_CONFIG_ADDR, EEPROM_CONFIG_PAGESIZE, i * 64, &tmp[0], 64);
		}
		dbgu_printf("Done!\r\n");
	}
}

void eeprom_writeRaw(unsigned char id, unsigned int startreg, const unsigned char* data, unsigned short length) {
	unsigned char addr = EEPROM_CONFIG_ADDR;
	unsigned short pagelen = EEPROM_CONFIG_PAGESIZE;
	if (id == 1) {
		addr = EEPROM_MOTION_ADDR_L;
		pagelen = EEPROM_MOTION_PAGESIZE;
	}
	eeprom_writeBytes(addr, pagelen, startreg, data, length);
}

void eeprom_readRaw(unsigned char id, unsigned int startreg, unsigned char* buffer, unsigned short length) {
	unsigned char addr = EEPROM_CONFIG_ADDR;
	if (id == 1) {
		addr = EEPROM_MOTION_ADDR_L;
	}
	if (startreg >= 0x10000) {
		addr |= 1;
		startreg -= 0x10000;
	}
	I2C_ReadMultipleRegisters2(addr, startreg, buffer, length);
}
