/*
 *   TauraTracker.cpp
 *
 *   Author: ROBOTIS
 *
 */

#include <math.h>
#include "Head.h"
#include "Camera.h"
#include "ImgProcess.h"
#include "TauraTracker.h"

using namespace Robot;


TauraTracker::TauraTracker() :
        ball_position(Point2D(-1.0, -1.0))
{
	NoBallCount = 0;
}

TauraTracker::~TauraTracker()
{
}

void TauraTracker::Process(Point2D pos)
{
	if(pos.X < 0 || pos.Y < 0)
	{
		ball_position.X = -1;
		ball_position.Y = -1;
		if(NoBallCount < NoBallMaxCount)
		{
			Head::GetInstance()->MoveTracking();
			NoBallCount++;
		}
		else
			Head::GetInstance()->InitTracking();
	}
	else
	{
		NoBallCount = 0;
		Point2D center = Point2D(Camera::WIDTH/2, Camera::HEIGHT/2);
		Point2D offset = pos - center;
		offset *= -1; // Inverse X-axis, Y-axis
		offset.X *= (Camera::VIEW_H_ANGLE / (double)Camera::WIDTH); // pixel per angle
		offset.Y *= (Camera::VIEW_V_ANGLE / (double)Camera::HEIGHT); // pixel per angle
		Head::GetInstance()->MoveTracking(offset);
		offset.X += Head::GetInstance()->GetPanAngle(); // sum neck angle
		offset.Y += Head::GetInstance()->GetTiltAngle() - 145.0; // sum neck angle
		offset.Y = ROBOT_HEIGHT * tan(offset.Y*M_PI/180.0); // transform into distance
		ball_position = offset;
	}
}
