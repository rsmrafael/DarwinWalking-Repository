/*
 * BlackboardTest.h
 *
 *  Created on: 09.08.2012
 *      Author: Stefan Krupop
 */

#ifndef BLACKBOARDTEST_H_
#define BLACKBOARDTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../../src/Blackboard/Blackboard.h"
#include "../../src/Blackboard/DataHolder.h"
#include "../../src/Representations/TestRepresentation.h"

class OtherTestRepresentation : public TestRepresentation {
};

class BlackboardTest : public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE(BlackboardTest);
		CPPUNIT_TEST(representationNotExistingTest);
		CPPUNIT_TEST(differentNamesTest);
		CPPUNIT_TEST(addTest);
		CPPUNIT_TEST(representationTypeMismatchTest);
	CPPUNIT_TEST_SUITE_END();

public:
	BlackboardTest() { bb = NULL; };

	void setUp(void);
	void tearDown(void);

protected:
	void representationNotExistingTest(void);
	void differentNamesTest(void);
	void addTest(void);
	void representationTypeMismatchTest(void);

private:
	Blackboard* bb;
};

#endif /* BLACKBOARDTEST_H_ */
