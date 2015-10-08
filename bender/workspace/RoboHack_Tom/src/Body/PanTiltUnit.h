/*
 * PanTiltUnit.h
 *
 *  Created on: 25.06.2009
 *      Author: Stefan
 */

#ifndef PANTILTUNIT_H_
#define PANTILTUNIT_H_

#include "ICommunicator.h"

/*!
 * PanTiltUnit handle the communication with the Pan and the tilt servo. Also make a correction wich can
 * be difference for each Robot, because the camera and basic position can be difference.
 */
class PanTiltUnit {
public:

	PanTiltUnit(ICommunicator* comm);//!< Constructor variable initalisation and set Camera to basic position.
	~PanTiltUnit();//!< Destructor handle mComm

	void SetPosition(double PanDeg, double TiltDeg);//!< Set the position of the Pan and Tilt servo.
	void SetPan(double PanDeg);//!< Set only the pan.
	void SetTilt(double TiltDeg);//!< Set only the tilt.
	void SetPanTiltError(double panErr, double tiltErr);//!< Pan and tilt correction set, for the difference to the standart position.
	void MoveToHome();//!< Move to basic position.

	double GetTilt() const;//!< Get the Tilt Value.
	double GetPan() const;//!< Get the Pan Value

	void InitTracking();//!< Reset all Error Variables that are used for the difference to the searched Object.
	void MoveTracking(bool disablePan);//!< Move to a position where the error is a little reduced.
	void MoveTracking(double errorX, double errorY, bool disablePan);//!< Move to position with difference of the error values

private:
	void Execute();//!< execute the standart so set the pantilt every time

	ICommunicator* mComm;/*!< Communication to the body */

	double mPan;/*!< The Pan to be set in Execute with error*/
    double mTilt;/*!< The Tilt to be set in Execute with error*/

	double mPanError;/*!< The Pan Error given by the Basic Pose*/
	double mTiltError;/*!< The Tilt Error given by the Basic Pose*/

	double mPanErr;/*!< The PanErr error to the Object*/
	double mPanErrDiff;/*!< The Pan Error Difference since the last run*/
	double mTiltErr;/*!< The Tilt Error given by the Basic Pose*/
	double mTiltErrDiff;/*!< The Tilt Error Difference since the last run*/

};

#endif /* PANTILTUNIT_H_ */
