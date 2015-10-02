#ifndef _FINDBALL_H_
#define _FINDBALL_H_

#include "cblob.h"
#include "Point.h"
#include "Image.h"
#include "FieldLimitsFinder.h"
#include "BlobScan.h"
#include <list>

/*
#define BALL_BRIGHTNESS_THRESHOLD 155
#define MAX_RADIUS_INDEX 12
#define MIN_RADIUS 5
#define MAX_RADIUS 110
#define RADIUS_STEP ((MAX_RADIUS - MIN_RADIUS) / MAX_RADIUS_INDEX)
#define ANGLE_STEP (2*M_PI / MAX_ANGLE_INDEX)
#define BALL_FINDER_SCAN_STEP 2
#define BALL_FINDER_MARGIN 20
#define MAX_ANGLE_INDEX 18
#define MAX_GRASS_INSIDE (2)
#define MIN_BRIGHT_INSIDE (12)
#define MIN_GRASS_OUTSIDE (12)
// Non-bright includes grass
#define MIN_NONBRIGHT_OUTSIDE (14)
#define MAX_SCAN_STEP 5
*/

#define BALL_BRIGHTNESS_THRESHOLD 165
#define MAX_RADIUS_INDEX 12
#define MIN_RADIUS 10
#define MAX_RADIUS 110
#define RADIUS_STEP ((MAX_RADIUS - MIN_RADIUS) / MAX_RADIUS_INDEX)
#define ANGLE_STEP (2*M_PI / MAX_ANGLE_INDEX)
//#define BALL_FINDER_SCAN_STEP 2
#define BALL_FINDER_MARGIN 20
#define MAX_ANGLE_INDEX 18
#define MAX_GRASS_INSIDE (2)
#define MIN_BRIGHT_INSIDE (12)
#define MIN_GRASS_OUTSIDE (12)
// Non-bright includes grass
#define MIN_NONBRIGHT_OUTSIDE (14)
#define MAX_SCAN_STEP 5

namespace Robot
{
    class Circle : public Point2D
    {
        public:
            Circle();
            Circle(double x, double y, double radius);
            double mRadius;
    };

    class BallFinder {
        public:
            BallFinder(Image *image, FieldLimitsFinder *fieldLimits);
            ~BallFinder();
            int PossibleBallRadiusAtPoint(int x, int y);
            void FindCandidates();
            void SetFieldLimitsFinder(FieldLimitsFinder *fieldLimits);
            std::list<Robot::Circle> GetBallCandidates()
                                            { return mBallCandidates; }
            std::list<Robot::Circle> mBallCandidates;
            void DrawCandidates(Image *image);
        private:
            FieldLimitsFinder *mFieldLimitsFinder;
            Image *mBallFinderImage;
            uint16_t *mFieldColorMap;
            int mCircle_dx[MAX_RADIUS_INDEX][MAX_ANGLE_INDEX];
            int mCircle_dy[MAX_RADIUS_INDEX][MAX_ANGLE_INDEX];
            int mScanStep[MAX_SCAN_STEP];
    };
}

#endif

