/*
 *   TauraTracker.h
 *
 *   Author: ROBOTIS
 *
 */

#ifndef _TAURA_TRACKER_H_
#define _TAURA_TRACKER_H_

#include <string.h>

#include "Point.h"
#include "minIni.h"

#define ROBOT_HEIGHT 50.0

namespace Robot
{
	class TauraTracker
	{
	private:
		int NoBallCount;
		static const int NoBallMaxCount = 15;

	public:
        Point2D     ball_position;

		TauraTracker();
		~TauraTracker();

		void Process(Point2D pos);
	};
}

#endif
