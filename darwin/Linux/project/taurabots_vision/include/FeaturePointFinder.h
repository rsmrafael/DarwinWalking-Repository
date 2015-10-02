#ifndef _FEATUREPOINTFINDER_H_
#define _FEATUREPOINTFINDER_H_

#include "Image.h"
#include "FeaturePoint.h"
#include "VisionUtil.h"
#include <list>

#define TOTAL_ANGLES 36
#define TOTAL_RADIUS 6
#define MIN_RADIUS 8
#define MAX_RADIUS 8
#define MARGIN 12
#define MAX_ANGLE_FOR_LINE 0.60
#define THRESHOLD 3*175
#define STEP 7
#define CONTRAST_DIST 12
#define MAX_CLUSTERS 50
#define MIN_CLUSTER_DIST (MAX_RADIUS)

namespace Robot
{
    class FeaturePointFinder {
        private:
            double dx[TOTAL_ANGLES][TOTAL_RADIUS];
            double dy[TOTAL_ANGLES][TOTAL_RADIUS];
            double mAngle[TOTAL_ANGLES];
            double mRadius[TOTAL_RADIUS];
            int mThreshold;
            void FindFeaturePoints(Image *image);
            void FindClusters(Image *image);
            unsigned int mTotals[TOTAL_TYPES];
            unsigned int mTotalClusters[TOTAL_TYPES];
            FeaturePoint mClusters[TOTAL_TYPES][MAX_CLUSTERS];
        public:
            FeaturePointFinder();
            // Goes through the image finding the feature points
            void ProcessFrame(Image *image);
            // Sets the binary image threshold
            void SetThreshold(int threshold)
                    { mThreshold = 3 * threshold; }
            // Gets the binary image threshold
            int GetThreshold() { return mThreshold / 3; };
            // Draw the feature points on the image buffer for debugging
            void DrawFeaturePoints(Image *image);
            // Finds the type of the feature point found
            FeaturePointType GetFeaturePointType(
                    int x0, int y0, Image *image,
                    double *angles);
            // This list will store the feature points found
            std::list<FeaturePoint> mPoint;
    };
}

#endif

