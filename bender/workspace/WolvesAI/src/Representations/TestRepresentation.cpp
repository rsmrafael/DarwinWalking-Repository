/*
 * TestRepresentation.cpp
 *
 *  Created on: 07.08.2012
 *      Author: Stefan Krupop
 */

#include "TestRepresentation.h"
#include "../Debugging/Debugger.h"

int TestRepresentation::cnt = 0;

TestRepresentation::TestRepresentation() {
	mInst = cnt;
	cnt++;

	//cout << "TestRepresentation::TestRepresentation() Nr. " << mInst << endl << flush;
	mTest = 23;
}

TestRepresentation::~TestRepresentation() {
	//cout << "TestRepresentation::~TestRepresentation() Nr. " << mInst << endl << flush;
}
