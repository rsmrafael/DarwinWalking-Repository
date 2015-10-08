/*
 * BlackboardTest.cpp
 *
 *  Created on: 09.08.2012
 *      Author: Stefan Krupop
 */

#include "BlackboardTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(BlackboardTest);

void BlackboardTest::setUp(void) {
	delete bb;
	bb = new Blackboard();
}

void BlackboardTest::tearDown(void) {
	delete bb;
}

void BlackboardTest::representationNotExistingTest(void) {
	CPPUNIT_ASSERT_THROW(const DataHolder<TestRepresentation>& test = const_cast<const Blackboard*>(bb)->getRepresentation<DataHolder<TestRepresentation> >("Test"), RepresentationNotExisting);
}

void BlackboardTest::differentNamesTest(void) {
	DataHolder<TestRepresentation>& test = bb->getRepresentation<DataHolder<TestRepresentation> >("Test1");
	(*test).setTest(10);
	DataHolder<TestRepresentation>& test1 = bb->getRepresentation<DataHolder<TestRepresentation> >("Test2");
	(*test1).setTest(20);

	const DataHolder<TestRepresentation>& test2 = bb->getRepresentation<DataHolder<TestRepresentation> >("Test1");
	CPPUNIT_ASSERT_EQUAL(10, (*test2).getTest());
	const DataHolder<TestRepresentation>& test3 = const_cast<const Blackboard*>(bb)->getRepresentation<DataHolder<TestRepresentation> >("Test2");
	CPPUNIT_ASSERT_EQUAL(20, (*test3).getTest());
}

void BlackboardTest::addTest(void) {
	bb->getRepresentation<DataHolder<TestRepresentation> >("Test3");
}

void BlackboardTest::representationTypeMismatchTest(void) {
	bb->getRepresentation<DataHolder<TestRepresentation> >("Test4");
	CPPUNIT_ASSERT_THROW(bb->getRepresentation<DataHolder<OtherTestRepresentation> >("Test4"), RepresentationTypeMismatch);
	CPPUNIT_ASSERT_THROW(const DataHolder<OtherTestRepresentation>& test = const_cast<const Blackboard*>(bb)->getRepresentation<DataHolder<OtherTestRepresentation> >("Test4"), RepresentationTypeMismatch);
}
