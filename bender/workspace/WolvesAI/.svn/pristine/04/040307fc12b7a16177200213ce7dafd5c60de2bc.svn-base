/*
 * TestRepresentation.h
 *
 *  Created on: 07.08.2012
 *      Author: Stefan Krupop
 */

#ifndef TESTREPRESENTATION_H_
#define TESTREPRESENTATION_H_

#include "../Blackboard/Printable.h"
#include <string>

using namespace std;

/**
 * Representation for Test
 */
class TestRepresentation : public Printable {
public:
	/**
	 * Constructor
	 */
	TestRepresentation();

	/**
	 * Destructor
	 */
	virtual ~TestRepresentation();

	/**
	 * get test data
	 */
	int getTest() const { return mTest; }

	/**
	 * set test data
	 * @param value		value
	 */
	void setTest(int value) { mTest = value; }

	/**
	 * print data to stream
	 * @param stream 	output stream
	 */
	virtual void print(ostream& stream) const {
		stream << "Test: " << mTest << endl;
	}

private:
	int mTest;		//!< test value
	int mInst;		//!< instance value

	static int cnt;	//!< instance counter
};

#endif /* TESTREPRESENTATION_H_ */
