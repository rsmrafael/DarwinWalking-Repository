/*
 * PanTiltTest.h
 *
 *  Created on: 28.12.2013
 *      Author: Oliver Krebs
 */

#include "PanTiltTest.h"
//#include "../../src/RobotData.h"

#define MIN_PAN_ANGLE       -135.0 //-90
#define MAX_PAN_ANGLE		+135.0 //+90   // schwenken

#define MIN_TILT_ANGLE 		  0.0
#define MAX_TILT_ANGLE       90.0  //kippen

CPPUNIT_TEST_SUITE_REGISTRATION(PanTiltTest);

PanTiltTest::PanTiltTest() {
	comm = NULL;
	unit = NULL;
}

void PanTiltTest::setUp(void) {
	comm = new TestCommunicator();
	unit = new PanTiltUnit(comm);
}

void PanTiltTest::tearDown(void) {
	delete unit;
	delete comm;
}

void PanTiltTest::panTest(void) {
	double pan = MAX_PAN_ANGLE;
	unit->SetPan( pan);
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Pan max fail", pan, unit->GetPan());

	pan = MIN_PAN_ANGLE;
	unit->SetPan( pan);
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Pan min fail", pan, unit->GetPan());
}

void PanTiltTest::panInvalidTest(void) {
	double pan = MAX_PAN_ANGLE + 1;
	unit->SetPan( pan);
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Pan max+1 fail", MAX_PAN_ANGLE, unit->GetPan());

	pan = MIN_PAN_ANGLE - 1;
	unit->SetPan( pan);
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Pan min-1 fail", MIN_PAN_ANGLE, unit->GetPan());
}

void PanTiltTest::tiltTest(void) {
	double tilt = MAX_TILT_ANGLE;
	unit->SetTilt( tilt);
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Tilt max fail", tilt, unit->GetTilt());

	tilt = MIN_TILT_ANGLE;
	unit->SetTilt( tilt);
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Tilt min fail", tilt, unit->GetTilt());
}

void PanTiltTest::tiltInvalidTest(void) {
	double tilt = MAX_TILT_ANGLE + 1;
	unit->SetTilt( tilt);
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Tilt max+1 fail", MAX_TILT_ANGLE, unit->GetTilt());

	tilt = MIN_TILT_ANGLE - 1;
	unit->SetTilt( tilt);
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Tilt min-1 fail", MIN_TILT_ANGLE, unit->GetTilt());
}

void PanTiltTest::panTiltTest(void) {
	double pan = MAX_PAN_ANGLE;
	double tilt = MAX_TILT_ANGLE;
	unit->SetPosition( pan, tilt);
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Pan max (tilt max) fail", pan, unit->GetPan());
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Tilt max (pan max) fail", tilt, unit->GetTilt());

	pan = MIN_PAN_ANGLE;
	tilt = MIN_TILT_ANGLE;
	unit->SetPosition( pan, tilt);
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Pan min (tilt min) fail", pan, unit->GetPan());
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Tilt min (pan min) fail", tilt, unit->GetTilt());

	pan = MIN_PAN_ANGLE;
	tilt = MAX_TILT_ANGLE;
	unit->SetPosition( pan, tilt);
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Pan min (tilt max) fail", pan, unit->GetPan());
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Tilt max (pan min) fail", tilt, unit->GetTilt());

	pan = MAX_PAN_ANGLE;
	tilt = MIN_TILT_ANGLE;
	unit->SetPosition( pan, tilt);
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Pan max (tilt min) fail", pan, unit->GetPan());
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Tilt min (pan max) fail", tilt, unit->GetTilt());
}

void PanTiltTest::panTiltInvalidTest(void) {
	double pan = MAX_PAN_ANGLE + 1.0;
	double tilt = MAX_TILT_ANGLE + 1.0;
	unit->SetPosition( pan, tilt);
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Pan max+1 (tilt max+1) fail", MAX_PAN_ANGLE, unit->GetPan());
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Tilt max+1 (pan max+1) fail", MAX_TILT_ANGLE, unit->GetTilt());

	pan = MIN_PAN_ANGLE - 1.0;
	tilt = MIN_TILT_ANGLE - 1.0;
	unit->SetPosition( pan, tilt);
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Pan min-1 (tilt min-1) fail", MIN_PAN_ANGLE, unit->GetPan());
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Tilt min-1 (pan min-1) fail", MIN_TILT_ANGLE, unit->GetTilt());

	pan = MAX_PAN_ANGLE + 1.0;
	tilt = MIN_TILT_ANGLE - 1.0;
	unit->SetPosition( pan, tilt);
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Pan max+1 (tilt min-1) fail", MAX_PAN_ANGLE, unit->GetPan());
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Tilt min-1 (pan max+1) fail", MIN_TILT_ANGLE, unit->GetTilt());

	pan = MIN_PAN_ANGLE - 1.0;
	tilt = MAX_TILT_ANGLE + 1.0;
	unit->SetPosition( pan, tilt);
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Pan min-1 (tilt max+1) fail", MIN_PAN_ANGLE, unit->GetPan());
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Tilt max+1 (pan min-1) fail", MAX_TILT_ANGLE, unit->GetTilt());
}
