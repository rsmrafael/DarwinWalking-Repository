#ifndef _VISIONUTIL_H_
#define _VISIONUTIL_H_

#include "Image.h"
#include "FeaturePoint.h"

#define ROBOT_VISION_HEIGHT 0.5

namespace Robot
{
    class VisionUtil {
        private:
            static VisionUtil* mSingleInstance;
        public:
            static VisionUtil *GetInstance()
                    { return mSingleInstance; }
            bool GetBinaryPixel(int x, int y,
                    Image *image, int threshold);
            int GetGreyPixel(int x, int y,
                    Image *image);
            double AngleDifference(double angle1, double angle2);
            double AngleWrap(double angle);
            void DrawAngle(int x0, int y0, double angle,
                    unsigned char r, unsigned char g, unsigned char b,
                    Image *image);
            void DrawLine(int x0, int y0, int x1, int y1,
                    unsigned char r, unsigned char g, unsigned char b,
                    Image *image);
            void DrawPixel(int x, int y,
                    unsigned char r, unsigned char g, unsigned char b,
                    Image *image);
            Point2D CoordImgToField(Point2D p);
    };
}

#endif

