/*
 * DataTest.h
 *
 *  Created on: 28.12.2013
 *      Author: Oliver Krebs
 */

#include "DataTest.h"
#include <stdint.h>

CPPUNIT_TEST_SUITE_REGISTRATION(DataTest);

DataTest::DataTest() {

}

void DataTest::setUp(void) {

}

void DataTest::tearDown(void) {

}

void DataTest::u16ToU8Test(void) {
	uint16_t val;
	uint8_t data1, data2;

	val = 0x0000;
	U16ToU8( val, data1, data2);
	CPPUNIT_ASSERT( data1 == 0x00);
	CPPUNIT_ASSERT( data2 == 0x00);
	U8ToU16( data1, data2, val);
	CPPUNIT_ASSERT( val == (uint16_t)0x0000);

	val = 0x1248;
	U16ToU8( val, data1, data2);
	CPPUNIT_ASSERT( data1 == 0x12);
	CPPUNIT_ASSERT( data2 == 0x48);
	U8ToU16( data1, data2, val);
	CPPUNIT_ASSERT( val == (uint16_t)0x1248);

	val = 0x8421;
	U16ToU8( val, data1, data2);
	CPPUNIT_ASSERT( data1 == 0x84);
	CPPUNIT_ASSERT( data2 == 0x21);
	U8ToU16( data1, data2, val);
	CPPUNIT_ASSERT( val == (uint16_t)0x8421);

	val = 0xFFFF;
	U16ToU8( val, data1, data2);
	CPPUNIT_ASSERT( data1 == 0xFF);
	CPPUNIT_ASSERT( data2 == 0xFF);
	U8ToU16( data1, data2, val);
	CPPUNIT_ASSERT( val == (uint16_t)0xFFFF);
}

void DataTest::s16ToU8Test(void) {
	int16_t val;
	uint8_t data1, data2;

	val = 0x0000;
	S16ToU8( val, data1, data2);
	CPPUNIT_ASSERT( data1 == 0x00);
	CPPUNIT_ASSERT( data2 == 0x00);
	U8ToS16( data1, data2, val);
	CPPUNIT_ASSERT( val == (int16_t)0x0000);

	val = 0x1248;
	S16ToU8( val, data1, data2);
	CPPUNIT_ASSERT( data1 == 0x12);
	CPPUNIT_ASSERT( data2 == 0x48);
	U8ToS16( data1, data2, val);
	CPPUNIT_ASSERT( val == (int16_t)0x1248);

	val = 0x8421;
	S16ToU8( val, data1, data2);
	CPPUNIT_ASSERT( data1 == 0x84);
	CPPUNIT_ASSERT( data2 == 0x21);
	U8ToS16( data1, data2, val);
	CPPUNIT_ASSERT( val == (int16_t)0x8421);

	val = 0xFFFF;
	S16ToU8( val, data1, data2);
	CPPUNIT_ASSERT( data1 == 0xFF);
	CPPUNIT_ASSERT( data2 == 0xFF);
	U8ToS16( data1, data2, val);
	CPPUNIT_ASSERT( val == (int16_t)0xFFFF);
}
