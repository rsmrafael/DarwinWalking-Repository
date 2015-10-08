/*
 * CommunicatorTest.h
 *
 *  Created on: 10.01.2014
 *      Author: Oliver Krebs
 */

#ifndef COMMUNICATORTEST_H_
#define COMMUNICATORTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../../src/Body/PanTiltUnit.h"
#include "../../src/Body/TestCommunicator.h"
#include "../../src/Body/Body.h"

class CommunicatorTest : public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE(CommunicatorTest);
		CPPUNIT_TEST(movementCmdTest);
		CPPUNIT_TEST(panTiltCmdTest);
		CPPUNIT_TEST(kickerCmdTest);
		CPPUNIT_TEST(walkerCmdTest);
		CPPUNIT_TEST(statusMessageImuTest);
		CPPUNIT_TEST(statusMessageVoltageTest);
	CPPUNIT_TEST_SUITE_END();

public:
	CommunicatorTest();

	void setUp(void);
	void tearDown(void);

protected:
	void movementCmdTest(void);
	void panTiltCmdTest(void);
	void kickerCmdTest(void);
	void walkerCmdTest(void);
	void statusMessageImuTest(void);
	void statusMessageVoltageTest(void);

private:
	Body *body;
	TestCommunicator *comm;
};

#endif
