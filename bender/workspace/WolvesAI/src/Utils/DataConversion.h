/*
 * DataConversion.h
 *
 *  Created on: 28.12.2013
 *      Author: Oliver Krebs
 */

#ifndef DATACONVERSION_H_
#define DATACONVERSION_H_

#include <stdint.h>

/**
 * convert a uint16 to 2 uint8 (byte)
 * @param val		input value
 * @param data1		output value 1 (higher byte)
 * @param data2		output value 2 (lower byte)
 */
inline void U16ToU8(uint16_t val, uint8_t &data1, uint8_t &data2) {
	data1 = (uint8_t)(((val & 0xFF00) >> 8) & 0xFF);
	data2 = (uint8_t)(val & 0xFF);
}

/**
 * convert a int16 to 2 uint8 (byte)
 * @param val		input value
 * @param data1		output value 1 (higher byte)
 * @param data2		output value 2 (lower byte)
 */
inline void S16ToU8(int16_t val, uint8_t &data1, uint8_t &data2) {
	data1 = (uint8_t)(((val & 0xFF00) >> 8) & 0xFF);
	data2 = (uint8_t)(val & 0xFF);
}

/**
 * convert 2 uint8 to a uint16
 * @param data1		input value 1 (higher byte)
 * @param data2		input value 2 (lower byte)
 * @param val		output value
 */
inline void U8ToU16(uint8_t data1, uint8_t data2, uint16_t &val) {
	val = (uint16_t) ((data1 & 0xFF) << 8) | (data2 & 0xFF);
}

/**
 * convert 2 uint8 to a int16
 * @param data1		input value 1 (higher byte)
 * @param data2		input value 2 (lower byte)
 * @param val		output value
 */
inline void U8ToS16(uint8_t data1, uint8_t data2, int16_t &val) {
	val = (int16_t) ((data1 & 0xFF) << 8) | (data2 & 0xFF);
}

#endif /* DATACONVERSION_H_ */
