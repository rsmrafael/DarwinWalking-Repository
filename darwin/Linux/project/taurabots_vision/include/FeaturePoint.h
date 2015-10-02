#ifndef _FEATUREPOINT_H_
#define _FEATUREPOINT_H_

#include "Point.h"

enum FeaturePointType {
    TYPE_UNKNOWN,
    TYPE_POINT,
    TYPE_POINT_ON_LINE,
    TYPE_POINT_ON_L,
    TYPE_POINT_ON_T,
    TYPE_POINT_ON_X,
    TOTAL_TYPES
};

namespace Robot
{
    class FeaturePoint : public Point2D
    {
        public:
            FeaturePoint();
            FeaturePoint(int x, int y);
            FeaturePoint(int x, int y,
                    FeaturePointType type);
            FeaturePoint(int x, int y,
                    FeaturePointType type,
                    double *angles);
            float DistanceTo(FeaturePoint p2);
            double mAngle[4];
            FeaturePointType mType;
            int mCluster;
    };

    class Line
    {
        public:
            Line(FeaturePoint p1, FeaturePoint p2);
            FeaturePoint m_p1;
            FeaturePoint m_p2;
            float m_angle;
            float DistanceTo(FeaturePoint p);
    };
}

#endif

