/*
 * DataConversion.h
 *
 *  Created on: 28.12.2013
 *      Author: Oliver Krebs
 */

#ifndef DATACONVERSION_H_
#define DATACONVERSION_H_

#include <stdint.h>

inline void U16ToU8(uint16_t val, uint8_t &data1, uint8_t &data2) {
	data1 = (uint8_t)(((val & 0xFF00) >> 8) & 0xFF);
	data2 = (uint8_t)(val & 0xFF);
}

inline void S16ToU8(int16_t val, uint8_t &data1, uint8_t &data2) {
	data1 = (uint8_t)(((val & 0xFF00) >> 8) & 0xFF);
	data2 = (uint8_t)(val & 0xFF);
}

inline void U8ToU16(uint8_t data1, uint8_t data2, uint16_t &val) {
	val = (uint16_t) ((data1 & 0xFF) << 8) | (data2 & 0xFF);
}

inline void U8ToS16(uint8_t data1, uint8_t data2, int16_t &val) {
	val = (int16_t) ((data1 & 0xFF) << 8) | (data2 & 0xFF);
}

#endif /* DATACONVERSION_H_ */
