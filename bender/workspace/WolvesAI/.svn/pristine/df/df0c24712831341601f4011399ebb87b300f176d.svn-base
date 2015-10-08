/*
 * GoalDetectionOpenCV.cpp
 *
 *  Created on: 20.02.2015
 *      Author: Oliver
 */

#include "GoalDetectionOpenCV.h"
#include "../../Config.h"

GoalDetectionOpenCV::GoalDetectionOpenCV()
:	minY( 100)
{

}

GoalDetectionOpenCV::~GoalDetectionOpenCV()
{

}

bool GoalDetectionOpenCV::execute()
{
	int width = getImage().getWidth();
	for( int x = 0; x < width; ++x) {
		int y = getFieldLines().getY(x);
		YUVImage::YUVData data = getImage().getValue(x, y);
		if( data.Y > minY) {

		}
	}
	return true;
}

void GoalDetectionOpenCV::configChanged()
{
	Debugger::INFO("GoalDetectionOpenCV", "config changed");

	ConfigFile *config = Config::getInstance();
	minY = config->get<int>("Vision", "Goal_minY", 100);
}
