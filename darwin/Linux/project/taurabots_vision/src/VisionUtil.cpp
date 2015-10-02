#include "VisionUtil.h"
#include "FeaturePoint.h"
#include <cmath>
#define DRAW_RADIUS (15)

using namespace Robot;

VisionUtil *VisionUtil::mSingleInstance = new VisionUtil();

bool VisionUtil::GetBinaryPixel(int x, int y, Image *image, int threshold)
{
    int offset = 3 * (x * image->m_Width + y);
    int r = (int)image->m_ImageData[offset];
    int g = (int)image->m_ImageData[offset + 1];
    int b = (int)image->m_ImageData[offset + 2];
    if (r+g+b >= threshold)
    {
        return true;
    } else {
        return false;
    }
}

int VisionUtil::GetGreyPixel(int x, int y, Image *image)
{
    int offset = 3 * (x * image->m_Width + y);
    int r = (int)image->m_ImageData[offset];
    int g = (int)image->m_ImageData[offset + 1];
    int b = (int)image->m_ImageData[offset + 2];
    return r+g+b;
}

double VisionUtil::AngleDifference(double angle1, double angle2)
{
    double angle_dif = angle1 - angle2;
    return AngleWrap(angle_dif);
}

double VisionUtil::AngleWrap(double angle)
{
    if (angle< -M_PI)
    {
        angle+= 2*M_PI;
    } else if (angle> M_PI)
    {
        angle-= 2*M_PI;
    }
    return angle;
}

void VisionUtil::DrawPixel(int x, int y,
        unsigned char r, unsigned char g, unsigned char b,
        Image *image)
{
    int offset = 3 * (x * image->m_Width + y);
    image->m_ImageData[offset]     = r;
    image->m_ImageData[offset + 1] = g;
    image->m_ImageData[offset + 2] = b;
}

void VisionUtil::DrawAngle(int x0, int y0, double angle,
        unsigned char r, unsigned char g, unsigned char b,
        Image *image)
{
    int x1 = x0 + DRAW_RADIUS*sin(angle);
    int y1 = y0 + DRAW_RADIUS*cos(angle);
    DrawLine(x0, y0, x1, y1, r, g, b, image);
}

void VisionUtil::DrawLine(int x0, int y0, int x1, int y1,
        unsigned char r, unsigned char g, unsigned char b,
        Image *image)
{
    // Bresenham's line algorithm

    if (x1 != x0)
    {
        double dx = (double)(x1 - x0);
        double dy = (double)(y1 - y0);
        int y_sign = (y1 > y0) - (y1 < y0);
        int x_sign = (x1 > x0) - (x1 < x0);

        double err = 0.0;
        double derr = fabs(dy / dx);
        int y = y0;
        for (int x = x0 ; x != x1 ; x+=x_sign)
        {
            DrawPixel(x,y,r,g,b,image);
            err = err + derr;
            while (err >= 0.5)
            {
                y = y + y_sign;
                DrawPixel(x,y,r,g,b,image);
                err -= 1.0;
            }
        }
    } else {
        int y_sign = (y1 > y0) - (y1 < y0);
        for (int y = y0 ; y != y1 ; y+=y_sign)
        {
            DrawPixel(x0,y,r,g,b,image);
        }
    }
}

Point2D VisionUtil::CoordImgToField(Point2D p)
{
    /*
    double pan  = p.Y * (Camera::VIEW_H_ANGLE / (double)Camera::WIDTH);
    double tilt = p.X * (Camera::VIEW_V_ANGLE / (double)Camera::HEIGHT);
    double dist = tan(tilt) * ROBOT_VISION_HEIGHT;
    Point2D q.X = pan;
    Point2D q.Y = dist;
    */
    Point2D q;
    return q;
}

