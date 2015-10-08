/*
 * PanTiltTest.h
 *
 *  Created on: 28.12.2013
 *      Author: Oliver Krebs
 */

#ifndef PANTILTTEST_H_
#define PANTILTTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../../src/Body/PanTiltUnit.h"
#include "../../src/Body/TestCommunicator.h"

class PanTiltTest : public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE(PanTiltTest);
		CPPUNIT_TEST(panTest);
		CPPUNIT_TEST(panInvalidTest);
		CPPUNIT_TEST(tiltTest);
		CPPUNIT_TEST(tiltInvalidTest);
		CPPUNIT_TEST(panTiltTest);
		CPPUNIT_TEST(panTiltInvalidTest);
	CPPUNIT_TEST_SUITE_END();

public:
	PanTiltTest();

	void setUp(void);
	void tearDown(void);

protected:
	void panTest(void);
	void panInvalidTest(void);
	void tiltTest(void);
	void tiltInvalidTest(void);
	void panTiltTest(void);
	void panTiltInvalidTest(void);

private:
	TestCommunicator *comm;
	PanTiltUnit *unit;
};

#endif
