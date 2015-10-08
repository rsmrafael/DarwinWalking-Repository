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

class TestRepresentation : public Printable {
public:
	TestRepresentation();
	virtual ~TestRepresentation();

	int getTest() const { return mTest; }
	void setTest(int value) { mTest = value; }

	virtual void print(ostream& stream) const {
		stream << "Test: " << mTest << endl;
	}

private:
	int mTest;
	int mInst;

	static int cnt;
};

#endif /* TESTREPRESENTATION_H_ */
