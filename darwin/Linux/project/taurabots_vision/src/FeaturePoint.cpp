#include "FeaturePoint.h"
#include <cmath>
#define SQR(x) ((x)*(x))

using namespace Robot;

FeaturePoint::FeaturePoint(int x, int y,
        FeaturePointType type, double *angles) : Point2D(x,y)
{
    mType = type;
    mCluster = 0;
    for (int i = 0 ; i < 4 ; i++)
        mAngle[i] = angles[i];
}

FeaturePoint::FeaturePoint(int x, int y,
        FeaturePointType type) : Point2D(x,y)
{
    mType = type;
    mCluster = 0;
    for (int i = 0 ; i < 4 ; i++)
        mAngle[i] = 0.0;
}

FeaturePoint::FeaturePoint(int x, int y) : Point2D(x,y)
{
    mType = TYPE_UNKNOWN;
    mCluster = 0;
}

FeaturePoint::FeaturePoint() : Point2D()
{
    mType = TYPE_UNKNOWN;
    mCluster = 0;
}

float FeaturePoint::DistanceTo(FeaturePoint p2)
{
    return sqrt((double)(SQR(X - p2.X) + SQR(Y - p2.Y)));
}

Line::Line(FeaturePoint p1, FeaturePoint p2)
{
    m_p1 = p1;
    m_p2 = p2;
    float x1 = p1.X;
    float y1 = p1.Y;
    float x2 = p2.X;
    float y2 = p2.Y;
    m_angle = atan2(y2-y1,x2-x1)*180.0/M_PI;
}

float Line::DistanceTo(FeaturePoint p)
{
    float x0 = p.X;
    float y0 = p.Y;
    float y1 = m_p1.Y;
    float x1 = m_p1.X;
    float x2 = m_p2.X;
    float y2 = m_p2.Y;
    return fabs(((y2 - y1)*x0 - (x2 - x1)*y0 + x2*y1 - y2*x1)
                  / sqrt(SQR(y2 - y1) + SQR(x2 - x1)));
}

