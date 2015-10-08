/*
 * HorizonDetection.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#include "HorizonDetection.h"
#include "../../Config.h"

HorizonDetection::HorizonDetection() {
	tiltCorrection = 12;

	Config::GetInstance()->registerConfigChangeHandler(this);
	this->configChanged();
}

HorizonDetection::~HorizonDetection() {
	Config::GetInstance()->removeConfigChangeHandler(this);
}

bool HorizonDetection::execute()
{
	//int pan = getBodyStatus().getPan();
	int tilt = getBodyStatus().getTilt(); //Angle

	double angleVertical = getRobotConfiguration().cameraOpeningVertical;

	int width = getImage().getWidth();
	int heigth = getImage().getHeight();

//	Debugger::DEBUG("HorizonDetection", "tilt: %i" ,tilt);
//	Debugger::DEBUG("HorizonDetection", "angle: %f" , angleVertical);

//	int modifier = tilt/angleVertical * heigth;

	//Debugger::DEBUG("HorizonDetection", "modifier: %i" , modifier);

	int y = (int) ((double)heigth/2.0 + (double)heigth * (double)(tilt + tiltCorrection)/angleVertical);

//	Debugger::DEBUG("HorizonDetection", "y: %i" , y);

	if (y >= 0 && y < heigth) {
		Point p1(0, y);
		Point p2(width - 1, y);

		getHorizon().updateLine(p1, p2); //lint --e{429} 'p1' and 'p2' lost the custody
		DRAW_LINE("Horizon", 0, y, width, y, DebugDrawer::CYAN);
	}

	return true;
}


void HorizonDetection::configChanged() {
	Debugger::INFO("HorizonDetection", "config changed");

	tiltCorrection = Config::GetInt("Support", "Horizon_TiltCorrection", 12);

}