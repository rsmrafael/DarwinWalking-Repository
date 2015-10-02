#include "BallFinder.h"
#include "VisionUtil.h"
#include <cmath>
#include <cstdio>

using namespace Robot;
using namespace std;

Circle::Circle() : Point2D()
{
    mRadius = 0;
}

Circle::Circle(double x, double y, double radius) : Point2D(x,y)
{
    mRadius = radius;
}

BallFinder::BallFinder(Image *image, FieldLimitsFinder *fieldLimits)
{
    //printf("BallFinder constructor\n");
    mBallFinderImage = image;
    mFieldLimitsFinder = fieldLimits;
    mFieldColorMap = mFieldLimitsFinder->GetFieldColorMap();

    for (int radius_i = 0 ; radius_i < MAX_RADIUS_INDEX ; radius_i++)
    {
        for (int angle_i = 0 ; angle_i < MAX_ANGLE_INDEX ; angle_i++)
        {
            double radius = MIN_RADIUS + radius_i*RADIUS_STEP;
            double angle = angle_i*ANGLE_STEP;
            mCircle_dx[radius_i][angle_i] = (int)(radius * sin(angle));
            mCircle_dy[radius_i][angle_i] = (int)(radius * cos(angle));
        }
    }

    mScanStep[0] = 11;
    mScanStep[1] = 7;
    mScanStep[2] = 5;
    mScanStep[3] = 3;
    mScanStep[4] = 2;
}

int BallFinder::PossibleBallRadiusAtPoint(int x, int y)
{
    //printf("PossibleBallRadiusAtPoint %d, %d\n", x, y);
    for (int radius_i = MAX_RADIUS_INDEX-3 ; radius_i > 0 ; radius_i--)
    {
        double radius = MIN_RADIUS + radius_i*RADIUS_STEP;
        if (x - radius < 0 || y - radius < 0 ||
            x + radius > mBallFinderImage->m_Height ||
            y + radius > mBallFinderImage->m_Width)
        {
            continue;
        }
        int total_grass_inside = 0;
        int total_bright_inside = 0;
        int total_nonbright_outside = 0;
        int total_grass_outside = 0;
        for (int radius_ii = 0 ; radius_ii <= radius_i ; radius_ii++)
        {
            for (int angle_i = 0 ; angle_i < MAX_ANGLE_INDEX ; angle_i++)
            {
                //double angle = angle_i*ANGLE_STEP;
                int x0 = x + mCircle_dx[radius_ii][angle_i];
                int y0 = y + mCircle_dy[radius_ii][angle_i];
                if (is_pixel_in_colormap(*((rgb_pixel_t *)(mBallFinderImage->m_ImageData +
                                ((x0 * mBallFinderImage->m_Width) + y0)*3)), mFieldColorMap))
                {
                    total_grass_inside++;
                    if (total_grass_inside > (radius_i+1)*MAX_GRASS_INSIDE)
                    {
                        //printf("PossibleBallRadiusAtPoint %d, %d discarded: %d nonbright\n", x, y, total_grass_inside);
                        break;
                    }
                } else if (VisionUtil::GetInstance()->GetBinaryPixel(
                    x0,y0,mBallFinderImage,BALL_BRIGHTNESS_THRESHOLD))
                {
                    total_bright_inside++;
                }
            }
        }
        if (total_grass_inside <= (radius_i+1)*MAX_GRASS_INSIDE &&
            total_bright_inside >= (radius_i+1)*MIN_BRIGHT_INSIDE)
        {
            /* */
            for (int angle_i = 0 ; angle_i < MAX_ANGLE_INDEX ; angle_i++)
            {
                int x0 = x + mCircle_dx[radius_i+2][angle_i];
                int y0 = y + mCircle_dy[radius_i+2][angle_i];
                bool grass = is_pixel_in_colormap(
                        *((rgb_pixel_t *)(mBallFinderImage->m_ImageData
                        + ((x0 * mBallFinderImage->m_Width) + y0)*3)), mFieldColorMap);
                bool bright = !VisionUtil::GetInstance()->GetBinaryPixel(
                                 x0,y0,mBallFinderImage,BALL_BRIGHTNESS_THRESHOLD);
                if ( grass || bright)
                {
                    total_nonbright_outside++;
                    if (total_nonbright_outside > MIN_NONBRIGHT_OUTSIDE)
                    {
                        break;
                    }
                    if (grass)
                    {
                        total_grass_outside++;
                    }
                }
            }
            if (total_nonbright_outside > MIN_NONBRIGHT_OUTSIDE &&
                total_grass_outside >= MIN_GRASS_OUTSIDE)
            {/* */
                return MIN_RADIUS + radius_i*RADIUS_STEP;
            }
        }
    }
    return -1;
}

BallFinder::~BallFinder()
{
}

void BallFinder::SetFieldLimitsFinder(FieldLimitsFinder *fieldLimits)
{
    mFieldLimitsFinder = fieldLimits;
}

void BallFinder::FindCandidates()
{
    mBallCandidates.clear();
    int scan_step_index = 0;
    while (mBallCandidates.size() < 1 &&
           scan_step_index < MAX_SCAN_STEP)
    {
        for (int x = BALL_FINDER_MARGIN ;
                 x <= mBallFinderImage->m_Height - BALL_FINDER_MARGIN ;
                 x += mScanStep[scan_step_index])
        {
            for (int y = BALL_FINDER_MARGIN ;
                     y <= mBallFinderImage->m_Width - BALL_FINDER_MARGIN ;
                     y += mScanStep[scan_step_index])
            {
                if (mFieldLimitsFinder->IsInside(x,y))
                {
                    int radius = PossibleBallRadiusAtPoint(x,y);
                    if (radius != -1)
                    {
                        mBallCandidates.push_back(
                              Circle((double)x,(double)y,(double)radius));
                    }
                }
            }
        }
        scan_step_index++;
    }
}

void BallFinder::DrawCandidates(Image *image)
{
    for (list<Circle>::iterator i = mBallCandidates.begin() ;
            i != mBallCandidates.end() ; i++)
    {
        double angle_step = M_PI/18.0;
        for (double angle = 0.0 ; angle < 2*M_PI ; angle += angle_step)
        {
            int x0 = (int)(i->X + i->mRadius * sin(angle));
            int y0 = (int)(i->Y + i->mRadius * cos(angle));
            int x1 = (int)(i->X + i->mRadius * sin(angle+angle_step));
            int y1 = (int)(i->Y + i->mRadius * cos(angle+angle_step));
            VisionUtil::GetInstance()->DrawLine(x0,y0,x1,y1,255,0,0,image);
        }
    }
}

