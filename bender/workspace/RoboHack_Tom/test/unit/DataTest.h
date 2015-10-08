/*
 * DataTest.h
 *
 *  Created on: 28.12.2013
 *      Author: Oliver Krebs
 */

#ifndef DATATEST_H_
#define DATATEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../../src/Utils/DataConversion.h"

class DataTest : public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE(DataTest);
		CPPUNIT_TEST(u16ToU8Test);
		CPPUNIT_TEST(s16ToU8Test);
	CPPUNIT_TEST_SUITE_END();

public:
	DataTest();

	void setUp(void);
	void tearDown(void);

protected:
	void u16ToU8Test(void);
	void s16ToU8Test(void);
};

#endif /* DATATEST_H_ */
