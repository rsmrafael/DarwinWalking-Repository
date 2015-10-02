#ifndef _POLE_FINDER_H_
#define _POLE_FINDER_H_

#include "FieldLimitsFinder.h"
#include "Image.h"
#include "FeaturePointFinder.h"
#include "FeaturePoint.h"

#define MIN_POINTS_TO_FORM_A_LINE 5
#define LINE_MIN_WHITE_RATIO 0.9
#define MAX_DIST_LINE_POINT 24
#define MAX_DIST_POINTS_ON_LINE 30
#define WHITE_STEPS 10
#define MIN_ANGLE_TO_MERGE_LINES 20.0
#define LINEFINDER_STEP 8
#define LINEFINDER_CONTRAST_DIST 16
#define LINEFINDER_THRESHOLD 180
#define GRADIENT_MARGIN 30
#define MAX_POLE_CANDIDATES 50
#define POLECANDIDATE_TOLERANCE_DEPTH 36
#define POLECANDIDATE_ANGLE_MARGIN 30.0

namespace Robot
{
    class LinesFinder
    {
        public:
            LinesFinder(FieldLimitsFinder *fieldLimitsFinder);
            void FindLines(Image *image);
            void FindPoleCandidates();
            void DrawLines(Image *image, int r = 255, int g = 0, int b = 0);
            void DrawPoleCandidates(Image *image, int r = 255, int g = 0, int b = 255);
            std::list<FeaturePoint> GetPoleCandidates()
                                                { return mPoleCandidates; }
        private:
            float WhiteRatio(Image *image, FeaturePoint p1, FeaturePoint p2);
            void FindExtremes(std::list<FeaturePoint> line,
                    FeaturePoint *p1, FeaturePoint *p2);
            void MergeLines();
            FieldLimitsFinder *mFieldLimitsFinder;
            FeaturePointFinder *mFeaturePointFinder;
            std::list<FeaturePoint> mPoint;
            std::list<Line> mLine;
            //FeaturePoint mPoleCandidates[MAX_POLE_CANDIDATES];
            std::list<FeaturePoint> mPoleCandidates;
            int mTotalPoleCandidates;
    };
}

#endif

