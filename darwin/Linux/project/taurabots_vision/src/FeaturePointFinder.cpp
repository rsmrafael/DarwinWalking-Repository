#include "FeaturePointFinder.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#define SQR(x) ((x)*(x))

using namespace Robot;
using namespace std;

FeaturePointFinder::FeaturePointFinder()
{
    double angleStep = 2*M_PI / (double)TOTAL_ANGLES;
    double radiusStep = (double)(MAX_RADIUS - MIN_RADIUS)
                                            / (double)TOTAL_RADIUS;
    double angle = 0.0;
    // Initialize radial offsets to save runtime performance
    for (int angle_i = 0 ; angle_i < TOTAL_ANGLES ; angle_i++)
    {
        double radius = (double)MIN_RADIUS;
        mAngle[angle_i] = angle;
        for (int radius_i = 0 ; radius_i < TOTAL_RADIUS ; radius_i++)
        {
            if (angle_i == 0)
                mRadius[radius_i] = radius;
            dx[angle_i][radius_i] = radius*sin(angle);
            dy[angle_i][radius_i] = radius*cos(angle);
            radius += radiusStep;
        }
        angle += angleStep;
    }
    mThreshold = THRESHOLD;
}

void FeaturePointFinder::DrawFeaturePoints(Image *image)
{
    for (std::list<FeaturePoint>::iterator i = mPoint.begin() ;
            i != mPoint.end() ; i++)
    {
        switch(i->mType)
        {
            case TYPE_UNKNOWN:
                break;
            case TYPE_POINT:
                VisionUtil::GetInstance()->DrawPixel(
                        i->X, i->Y, 0, 255, 255, image);
                break;
            case TYPE_POINT_ON_LINE:
                VisionUtil::GetInstance()->DrawAngle(
                        i->X, i->Y, i->mAngle[0], 255, 0, 0, image);
                VisionUtil::GetInstance()->DrawAngle(
                        i->X, i->Y, i->mAngle[1], 255, 0, 0, image);
                break;
            case TYPE_POINT_ON_L:
                VisionUtil::GetInstance()->DrawAngle(
                        i->X, i->Y, i->mAngle[0], 0, 255, 0, image);
                VisionUtil::GetInstance()->DrawAngle(
                        i->X, i->Y, i->mAngle[1], 0, 255, 0, image);
                break;
            case TYPE_POINT_ON_T:
                VisionUtil::GetInstance()->DrawAngle(
                        i->X, i->Y, i->mAngle[0], 0, 0, 255, image);
                VisionUtil::GetInstance()->DrawAngle(
                        i->X, i->Y, i->mAngle[1], 0, 0, 255, image);
                VisionUtil::GetInstance()->DrawAngle(
                        i->X, i->Y, i->mAngle[2], 0, 0, 255, image);
                break;
            case TYPE_POINT_ON_X:
                VisionUtil::GetInstance()->DrawAngle(
                        i->X, i->Y, i->mAngle[0], 255, 0, 255, image);
                VisionUtil::GetInstance()->DrawAngle(
                        i->X, i->Y, i->mAngle[1], 255, 0, 255, image);
                VisionUtil::GetInstance()->DrawAngle(
                        i->X, i->Y, i->mAngle[2], 255, 0, 255, image);
                VisionUtil::GetInstance()->DrawAngle(
                        i->X, i->Y, i->mAngle[3], 255, 0, 255, image);
                break;
            case TOTAL_TYPES:
                break;
        }
    }
}

void FeaturePointFinder::FindFeaturePoints(Image *image)
{
    mPoint.clear();
    for (int i = 0 ; i < TOTAL_TYPES ; i++)
        mTotals[i] = 0;
    for (int i = MARGIN ; i < image->m_Height-MARGIN ; i+=STEP)
    {
        for (int j = MARGIN ; j < image->m_Width-MARGIN ; j+=STEP)
        {
            // nw, n, ne
            //  w, x, e
            // sw, s, se
            //printf("8-neighbours\n");
            bool nw = VisionUtil::GetInstance()->GetBinaryPixel(
                    i-CONTRAST_DIST,j-CONTRAST_DIST, image, mThreshold);
            bool n  = VisionUtil::GetInstance()->GetBinaryPixel(
                    i,              j-CONTRAST_DIST, image, mThreshold);
            bool ne = VisionUtil::GetInstance()->GetBinaryPixel(
                    i+CONTRAST_DIST,j-CONTRAST_DIST, image, mThreshold);
            bool w  = VisionUtil::GetInstance()->GetBinaryPixel(
                    i-CONTRAST_DIST,j              , image, mThreshold);
            bool x  = VisionUtil::GetInstance()->GetBinaryPixel(
                    i              ,j              , image, mThreshold);
            bool e  = VisionUtil::GetInstance()->GetBinaryPixel(
                    i+CONTRAST_DIST,j              , image, mThreshold);
            bool sw = VisionUtil::GetInstance()->GetBinaryPixel(
                    i-CONTRAST_DIST,j+CONTRAST_DIST, image, mThreshold);
            bool s  = VisionUtil::GetInstance()->GetBinaryPixel(
                    i,              j+CONTRAST_DIST, image, mThreshold);
            bool se = VisionUtil::GetInstance()->GetBinaryPixel(
                    i+CONTRAST_DIST,j+CONTRAST_DIST, image, mThreshold);
            if (x && ((!nw && !se) || //  '\' 
                      (!w  && !e)  || //  '-'
                      (!sw && !ne) || //  '/'
                      (!n  && !s)))   //  '|'
            {
                // add point at i,j as candidate point
                double angles[4];
                //printf("GetFeturePointType\n");
                FeaturePointType type =
                    GetFeaturePointType(i, j, image, angles);
                mTotals[type]++;
                mPoint.push_back(FeaturePoint(i,j,type,angles));
            }
        }
    }
}

FeaturePointType FeaturePointFinder::GetFeaturePointType(int x0, int y0,
                                     Image *image, double *angles)
{
    bool radials[TOTAL_ANGLES];
    int lastFalseRadial = -1;
    int firstTrueRadial = -1;
    for (int angle_i = 0 ; angle_i < TOTAL_ANGLES ; angle_i++)
    {
        radials[angle_i] = true;
        for (int radius_i = 0 ; radius_i < TOTAL_RADIUS ; radius_i++)
        {
            int x = x0 + dx[angle_i][radius_i];
            int y = y0 + dy[angle_i][radius_i];
            //if (!VisionUtil::GetInstance()->GetBinaryPixel(
            //          x  ,y  ,image, mThreshold))
            if (!(VisionUtil::GetInstance()->GetBinaryPixel(
                      x  ,y  ,image, mThreshold) || 
                  VisionUtil::GetInstance()->GetBinaryPixel(
                      x-1,y  ,image, mThreshold) ||
                  VisionUtil::GetInstance()->GetBinaryPixel(
                      x  ,y-1,image, mThreshold) ||
                  VisionUtil::GetInstance()->GetBinaryPixel(
                      x+1,y  ,image, mThreshold) ||
                  VisionUtil::GetInstance()->GetBinaryPixel(
                      x  ,y+1,image, mThreshold) ||
                  VisionUtil::GetInstance()->GetBinaryPixel(
                      x+1,y+1,image, mThreshold) ||
                  VisionUtil::GetInstance()->GetBinaryPixel(
                      x+1,y-1,image, mThreshold) ||
                  VisionUtil::GetInstance()->GetBinaryPixel(
                      x-1,y+1,image, mThreshold) ||
                  VisionUtil::GetInstance()->GetBinaryPixel(
                      x-1,y-1,image, mThreshold)))
            {
                radials[angle_i] = false;
                break;
            }
        }
        if (radials[angle_i] && firstTrueRadial == -1)
        {
            firstTrueRadial = angle_i;
        } else if (!radials[angle_i])
        {
            lastFalseRadial = angle_i;
        }
    }
    // No radial, then type is unknown
    if (firstTrueRadial == -1) return TYPE_POINT;
    // All radials, then type is point
    if (lastFalseRadial == -1) return TYPE_UNKNOWN;
    // Wrap around to find the true first radial
    if (lastFalseRadial < TOTAL_ANGLES-1)
    {
        firstTrueRadial = lastFalseRadial+1;
    }
    // Mark the first radial angle
    double startAngle = mAngle[firstTrueRadial];
    double finishAngle;
    int count = 0;
    // Set up a search for the remaining radials (if any)
    int angle_i_before = firstTrueRadial;
    int angle_i = (angle_i_before + 1) % TOTAL_ANGLES;
    do
    {
        if (radials[angle_i_before] == true &&
            radials[angle_i] == false)
        {
            finishAngle = mAngle[angle_i];
            angles[count] = VisionUtil::GetInstance()->AngleWrap(
                    startAngle + VisionUtil::GetInstance()->AngleDifference(
                        finishAngle, startAngle) / 2.0);
            count++;
            if (count == 5)
                break;
        }
        if (radials[angle_i_before] == false &&
            radials[angle_i] == true)
        {
            startAngle = mAngle[angle_i];
        }
        angle_i_before = angle_i;
        angle_i = (angle_i + 1) % TOTAL_ANGLES;
    } while (angle_i != firstTrueRadial);
    // If found two radials, then type is either
    // point on line or point on L depending on
    // the angle difference
    if (count == 2)
    {
        double angle_dif = VisionUtil::GetInstance()->GetInstance()->AngleDifference(angles[0], angles[1]);
        if (fabs(fabs(angle_dif) - M_PI) < MAX_ANGLE_FOR_LINE)
        {
            return TYPE_POINT_ON_LINE;
        } else {
            return TYPE_POINT_ON_L;
        }
    // If found three radials then is a T
    } else if (count == 3)
    {
        return TYPE_POINT_ON_T;
    // If found three radials then is an X
    } else if (count == 4)
    {
        return TYPE_POINT_ON_X;
    }
    // If none of the above, then unknown
    return TYPE_UNKNOWN;
}

void FeaturePointFinder::ProcessFrame(Image *image)
{
    //printf("FindFeaturePoints\n");
    FindFeaturePoints(image);
    int count = 0;
    while (count < 5)
    {
        count++;
        //printf("FindClusters\n");
        FindClusters(image);
    }
}

void FeaturePointFinder::FindClusters(Image *image)
{
    double total_X[MAX_CLUSTERS];
    double total_Y[MAX_CLUSTERS];
    double total_angles[MAX_CLUSTERS][4];
    int total[MAX_CLUSTERS];
    int angle_max = 2;
    for (int type = 1 ; type < TOTAL_TYPES ; type++)
    {
        if (type == TYPE_POINT_ON_T) angle_max = 3;
        else if (type == TYPE_POINT_ON_X) angle_max = 4;
        mTotalClusters[type] = MAX_CLUSTERS;
        for (unsigned int k = 0 ; k < mTotalClusters[type] ; k++)
        {
            FeaturePoint p(rand() % image->m_Height,
                           rand() % image->m_Width, (FeaturePointType)type);
            mClusters[type][k] = p;
        }
        bool not_finished = true;
        int count = 0;
        while (not_finished && count < 15)
        {
            not_finished = false;
            //printf("type:%d, clusters:%d, count %d\n", type, mTotalClusters[type], count);
            for (unsigned int k = 0 ; k < mTotalClusters[type] ; k++)
            {
                total_X[k] = 0.0;
                total_Y[k] = 0.0;
                total[k] = 0;
                for (int angle_i = 0 ; angle_i < angle_max ; angle_i++)
                    total_angles[k][angle_i] = 0.0;
            }
            for (std::list<FeaturePoint>::iterator i = mPoint.begin() ;
                 i != mPoint.end() ; i++)
            {
                if (i->mType == (FeaturePointType)type)
                {
                    bool firstRun = true;
                    float minDist = 0.0;
                    int min_k = 0;
                    for (unsigned int k = 0 ; k < mTotalClusters[type] ; k++)
                    {
                        float dist = i->DistanceTo(mClusters[type][k]);
                        if (dist < minDist || firstRun)
                        {
                            firstRun = false;
                            minDist = dist;
                            min_k = k;
                        }
                    }
                    total[min_k]++;
                    total_X[min_k] += i->X;
                    total_Y[min_k] += i->Y;
                    for (int angle_i = 0 ; angle_i < angle_max ; angle_i++)
                    {
                        total_angles[min_k][angle_i] += i->mAngle[angle_i];
                    }
                }
            }
            for (unsigned int k = 0 ; k < mTotalClusters[type] ; k++)
            {
                if (total[k] > 0)
                {
                    total_X[k] /= (double)total[k];
                    total_Y[k] /= (double)total[k];
                    for (int angle_i = 0 ; angle_i < angle_max ; angle_i++)
                    {
                        total_angles[k][angle_i] /= (double)total[k];
                    }
                } else {
                    total_X[k] = -1;
                }
                if (mClusters[type][k].X != total_X[k] ||
                    mClusters[type][k].Y != total_Y[k])
                {
                    not_finished = true;
                    mClusters[type][k].X = total_X[k];
                    mClusters[type][k].Y = total_Y[k];
                    for (int angle_i = 0 ; angle_i < 4 ; angle_i++)
                    {
                        mClusters[type][k].mAngle[angle_i] =
                                            total_angles[k][angle_i];
                    }
                    mClusters[type][k].mType = (FeaturePointType)type;
                    mClusters[type][k].mCluster++;
                }
            }
            count++;
        }
    }
    for (int type = 1 ; type < TOTAL_TYPES ; type++)
    {
        for (unsigned int k = 0 ; k < mTotalClusters[type] ; k++)
        {
            if (mClusters[type][k].X != -1)
            {
                bool firstRun = true;
                float minDist = 0.0;
                FeaturePoint min_p;
                for (std::list<FeaturePoint>::iterator i = mPoint.begin() ;
                     i != mPoint.end() ; i++)
                {
                    if (i->mType == (FeaturePointType)type)
                    {
                        float dist = i->DistanceTo(mClusters[type][k]);
                        if (dist < minDist || firstRun)
                        {
                            firstRun = false;
                            minDist = dist;
                            min_p = *i;
                        }
                    }
                }
                if (!firstRun)
                {
                    min_p.mCluster = mClusters[type][k].mCluster;
                    mClusters[type][k] = min_p;
                }
            }
        }
    }
    mPoint.clear();
    for (int type = 1 ; type < TOTAL_TYPES ; type++)
    {
        for (unsigned int k = 0 ; k < mTotalClusters[type] ; k++)
        {
            if (mClusters[type][k].X != -1)
            {
                mPoint.push_back(mClusters[type][k]);
                //printf("Adding point %f, %f type %d\n",
                //        mClusters[type][k].X, mClusters[type][k].Y, type);
            }
        }
    }
    if (mPoint.size() > 0)
    {
        for (std::list<FeaturePoint>::iterator i = mPoint.begin() ;
             i != mPoint.end() ; i++)
        {
            for (std::list<FeaturePoint>::iterator j = i ;
                 j != mPoint.end() ; j++)
            {
                float d = i->DistanceTo(*j);
                if (d < MIN_CLUSTER_DIST && j != i)
                {
                    if (i->mCluster > j->mCluster)
                    {
                        j->mCluster = 0;
                    } else if (i->mCluster < j->mCluster) {
                        i->mCluster = 0;
                    } else if ((int)i->mType > (int)j->mType)
                    {
                        j->mCluster = 0;
                    } else {
                        i->mCluster = 0;
                    }
                }
            }
        }
        for (std::list<FeaturePoint>::iterator i = mPoint.begin() ;
             i != mPoint.end() ; i++)
        {
            if (i->mCluster == 0)
            {
                i = mPoint.erase(i);
            }
        }
    }
}

