#include "LinesFinder.h"
#include "VisionUtil.h"
#include <cmath>
#include <cstdio>

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

using namespace Robot;

LinesFinder::LinesFinder(FieldLimitsFinder *fieldLimitsFinder)
{
    mFieldLimitsFinder = fieldLimitsFinder;
    mFeaturePointFinder = new FeaturePointFinder();
    //mTotalPoleCandidates = 0;
}

void LinesFinder::FindLines(Image *image)
{
    mLine.clear();
    int threshold = 3*LINEFINDER_THRESHOLD;
    // First find candidate points
    for (int i = MARGIN ; i < image->m_Height-MARGIN ; i+=LINEFINDER_STEP)
    {
        for (int j = MARGIN ; j < image->m_Width-MARGIN ; j+=LINEFINDER_STEP)
        {
            // nw, n, ne
            //  w, x, e
            // sw, s, se
            int nw = VisionUtil::GetInstance()->GetGreyPixel(
                    i-LINEFINDER_CONTRAST_DIST,j-LINEFINDER_CONTRAST_DIST,image);
            int n  = VisionUtil::GetInstance()->GetGreyPixel(
                    i,              j-LINEFINDER_CONTRAST_DIST,image);
            int ne = VisionUtil::GetInstance()->GetGreyPixel(
                    i+LINEFINDER_CONTRAST_DIST,j-LINEFINDER_CONTRAST_DIST,image);
            int w  = VisionUtil::GetInstance()->GetGreyPixel(
                    i-LINEFINDER_CONTRAST_DIST,j,image);
            int x  = VisionUtil::GetInstance()->GetGreyPixel(
                    i,j,image);
            int e  = VisionUtil::GetInstance()->GetGreyPixel(
                    i+LINEFINDER_CONTRAST_DIST,j,image);
            int sw = VisionUtil::GetInstance()->GetGreyPixel(
                    i-LINEFINDER_CONTRAST_DIST,j+LINEFINDER_CONTRAST_DIST,image);
            int s  = VisionUtil::GetInstance()->GetGreyPixel(
                    i,j+LINEFINDER_CONTRAST_DIST, image);
            int se = VisionUtil::GetInstance()->GetGreyPixel(
                    i+LINEFINDER_CONTRAST_DIST,j+LINEFINDER_CONTRAST_DIST,image);
            int margin = x - GRADIENT_MARGIN;
            if (x > threshold && ((nw < margin && se < margin) || //  '\' 
                      (w < margin  && e < margin)  || //  '-'
                      (sw < margin && ne < margin) || //  '/'
                      (n < margin && s < margin)))   //  '|'
            {
                // add point at i,j as candidate point
                // double angles[4];
                //FeaturePointType type =
                //    mFeaturePointFinder->GetFeaturePointType(i, j, image, angles);
                //if (type == TYPE_POINT_ON_LINE)
                //{
                    mPoint.push_back(FeaturePoint(i,j));
                    //VisionUtil::GetInstance()->DrawPixel(i, j, 255, 0, 255, image);
                    //int color = x + 20;
                    //if (color > 255) color = 255;
                    //VisionUtil::GetInstance()->DrawLine(i-5, j-5, i+5, j+5, color, x-38, color, image);
                    //VisionUtil::GetInstance()->DrawLine(i-5, j+5, i+5, j-5, color, x-38, color, image);
                //}
            }
        }
    }
    //printf("LinesFinder candidate points found\n");

    std::list<FeaturePoint> lines;
    std::list<std::list<FeaturePoint>::iterator> line_candidates;
    FeaturePoint p1, p2;
    while (mPoint.size() > 0)
    {
        //printf("LinesFinder while (mPoint.size() > 0)\n");
        unsigned int size_before;
        lines.clear();
        lines.push_back(mPoint.front());
        mPoint.pop_front();
        bool lookingForMore = true;
        while (lookingForMore)
        {
            //printf("LinesFinder while (lookingForMore)\n");
            size_before = lines.size();
            if (size_before < 2)
            {
                //printf("size_before < 2\n");
                p1 = p2 = lines.front();
            } else {
                //printf("size_before > 2\n");
                FindExtremes(lines, &p1, &p2);
            }
            //printf("create line\n");
            Line line(p1,p2);
            //bool firstRun = true;
            line_candidates.clear();
            for (std::list<FeaturePoint>::iterator i = mPoint.begin() ;
                 i != mPoint.end() ; i++)
            {
                float dist = i->DistanceTo(p1);
                //printf("dist to p1\n");
                if (dist < MAX_DIST_POINTS_ON_LINE) // || firstRun)
                {
                    //printf("dist to p1 good!\n");
                    line_candidates.push_back(i);
                    //firstRun = false;
                }
            }

            //printf("inside line_candidates loop\n");
            for (std::list<std::list<FeaturePoint>::iterator>::iterator
                    j = line_candidates.begin() ; j != line_candidates.end() ; j++)
            {
                FeaturePoint candidate = **j;
                bool closeToLine = true;
                if (size_before >= 2)
                {
                    //printf("size_before >= 2\n");
                    int d = line.DistanceTo(candidate);
                    if (d > MAX_DIST_LINE_POINT)
                    {
                        closeToLine = false;
                        //printf("distance to line bad! %d\n",d);
                    }// else {
                    //    printf("distance to line good! %d\n",d);
                    //}
                }
                float wr = WhiteRatio(image, p1, candidate);
                //printf("wr = %f\n", wr);
                if (wr > LINE_MIN_WHITE_RATIO
                    && closeToLine)
                {
                    //printf("adding line point %d!\n",lines.size()+1);
                    lines.push_back(candidate);
                    //printf("erasing point\n");
                    mPoint.erase(*j);
                    //printf("erased point\n");
                }
            }

            if (lines.size() == size_before) lookingForMore = false;
        } // while (lookingForMore)

        if (size_before > MIN_POINTS_TO_FORM_A_LINE)
        {
            mLine.push_back(Line(p1,p2));
        }
    }

    //DrawLines(image,0,0,255);

    // Merge the lines found
    MergeLines();
}

void LinesFinder::FindExtremes(std::list<FeaturePoint> line,
                  FeaturePoint *p1, FeaturePoint *p2)
{
    FeaturePoint max_x;
    FeaturePoint min_x;
    FeaturePoint max_y;
    FeaturePoint min_y;
    bool firstRun = true;
    for (std::list<FeaturePoint>::iterator i = line.begin() ;
         i != line.end() ; i++)
    {
        if (firstRun || i->X > max_x.X) max_x = *i;
        if (firstRun || i->X < min_x.X) min_x = *i;
        if (firstRun || i->Y > max_y.Y) max_y = *i;
        if (firstRun || i->Y < min_y.Y) min_y = *i;
        if (firstRun) firstRun = false;
    }
    if (max_x.X - min_x.X > max_y.Y - min_y.Y)
    {
        *p1 = max_x;
        *p2 = min_x;
    } else {
        *p1 = max_y;
        *p2 = min_y;
    }
}

float LinesFinder::WhiteRatio(Image *image, FeaturePoint p1, FeaturePoint p2)
{
    int x0 = (p1.X < p2.X ? p1.X : p2.X);
    int dx = (int)(fabs(p2.X - p1.X) / (float)WHITE_STEPS);
    int y0 = (p1.Y < p2.Y ? p1.Y : p2.Y);
    int dy = (int)(fabs(p2.Y - p1.Y) / (float)WHITE_STEPS);
    int white = 0;
    int threshold = VisionUtil::GetInstance()->GetGreyPixel(x0,y0,image) - 5;
    for (int i = 0 ; i < WHITE_STEPS ; i++)
    {
        int x = x0 + i*dx;
        int y = y0 + i*dy;
        if (VisionUtil::GetInstance()->GetBinaryPixel(x, y, image, threshold))
        {
            white++;
        }
    }
    return (float)white / (float)(WHITE_STEPS);
}

void LinesFinder::MergeLines()
{
    bool keepMerging = true;
    while (keepMerging)
    {
        //printf("while (keepMerging)\n");
        unsigned int size_before = mLine.size();
        std::list<Line>::iterator i = mLine.begin();
        while (i != mLine.end())
        {
            //printf("while (i != mLine.end())\n");
            std::list<Line>::iterator j = mLine.begin();
            while (j != mLine.end())
            {
                if (i == mLine.end()) break;
                //printf("while (j != mLine.end())\n");
                if (i != j)
                {
                    int d1 = i->DistanceTo(j->m_p1);
                    int d2 = i->DistanceTo(j->m_p2);
                    int d3 = j->DistanceTo(i->m_p1);
                    int d4 = j->DistanceTo(i->m_p2);
                    int min_d = MIN(d1,MIN(d2,MIN(d3,d4)));
                    if (min_d < MAX_DIST_LINE_POINT)
                    {
                        float angle_diff = fabs(i->m_angle - j->m_angle);
                        if (angle_diff < MIN_ANGLE_TO_MERGE_LINES)
                        {
                            float min_x = MIN(i->m_p1.X,MIN(i->m_p2.X,MIN(j->m_p1.X,j->m_p2.X)));
                            float min_y = MIN(i->m_p1.Y,MIN(i->m_p2.Y,MIN(j->m_p1.Y,j->m_p2.Y)));
                            float max_x = MAX(i->m_p1.X,MAX(i->m_p2.X,MAX(j->m_p1.X,j->m_p2.X)));
                            float max_y = MAX(i->m_p1.Y,MAX(i->m_p2.Y,MAX(j->m_p1.Y,j->m_p2.Y)));
                            FeaturePoint p1,p2;
                            if (max_x - min_x > max_y - min_y)
                            {
                                if (i->m_p1.X == min_x)
                                {
                                    p1 = i->m_p1;
                                } else if (i->m_p2.X == min_x)
                                {
                                    p1 = i->m_p2;
                                } else if (j->m_p1.X == min_x)
                                {
                                    p1 = j->m_p1;
                                } else if (j->m_p2.X == min_x)
                                {
                                    p1 = j->m_p2;
                                }
                                if (i->m_p1.X == max_x)
                                {
                                    p2 = i->m_p1;
                                } else if (i->m_p2.X == max_x)
                                {
                                    p2 = i->m_p2;
                                } else if (j->m_p1.X == max_x)
                                {
                                    p2 = j->m_p1;
                                } else if (j->m_p2.X == max_x)
                                {
                                    p2 = j->m_p2;
                                }
                            } else {
                                if (i->m_p1.Y == min_y)
                                {
                                    p1 = i->m_p1;
                                } else if (i->m_p2.Y == min_y)
                                {
                                    p1 = i->m_p2;
                                } else if (j->m_p1.Y == min_y)
                                {
                                    p1 = j->m_p1;
                                } else if (j->m_p2.Y == min_y)
                                {
                                    p1 = j->m_p2;
                                }
                                if (i->m_p1.Y == max_y)
                                {
                                    p2 = i->m_p1;
                                } else if (i->m_p2.Y == max_y)
                                {
                                    p2 = i->m_p2;
                                } else if (j->m_p1.Y == max_y)
                                {
                                    p2 = j->m_p1;
                                } else if (j->m_p2.Y == max_y)
                                {
                                    p2 = j->m_p2;
                                }
                            }
                            Line l(p1,p2);
                            *i = l;
                            mLine.erase(j);
                            //mLine.erase(i);
                            //mLine.push_back(Line(p1,p2));
                            i = mLine.begin();
                            j = mLine.begin();
                            break;
                        }
                    }
                }
                j++;
            }
            i++;
        }
        if (size_before == mLine.size())
        {
            keepMerging = false;
        }
    }
}

void LinesFinder::DrawLines(Image *image, int r, int g, int b)
{
    for (std::list<Line>::iterator i = mLine.begin() ; i != mLine.end() ; i++)
    {
        int x0 = i->m_p1.X;
        int y0 = i->m_p1.Y;
        int x1 = i->m_p2.X;
        int y1 = i->m_p2.Y;
        VisionUtil::GetInstance()->DrawLine(x0,y0,x1,y1,r,g,b,image);
    }
}

void LinesFinder::FindPoleCandidates()
{
    mPoleCandidates.clear();
    float angle;
    for (std::list<Line>::iterator i = mLine.begin() ;
         i != mLine.end() ; i++)
    {
        if ( mFieldLimitsFinder->IsInside(i->m_p1.X+POLECANDIDATE_TOLERANCE_DEPTH,i->m_p1.Y) &&
            !mFieldLimitsFinder->IsInside(i->m_p2.X,i->m_p2.Y))
        {
            if (fabs(i->m_angle) >= 90.0)
            {
                angle = i->m_angle - 180.0;
            } else {
                angle = i->m_angle;
            }
            while (angle < -180.0) angle += 360.0;
            while (angle > 180.0) angle -= 360.0;
            if ((angle >= 0 && angle <  POLECANDIDATE_ANGLE_MARGIN) ||
                (angle < 0 && angle > -POLECANDIDATE_ANGLE_MARGIN))
            {
                //printf("dentro angle %f\n",angle);
                //mPoleCandidates[mTotalPoleCandidates] = i->m_p1;
                //mTotalPoleCandidates++;
                mPoleCandidates.push_back(i->m_p1);
            } //else {
              //  printf("fora angle %f\n",angle);
              //}
        } else if ( mFieldLimitsFinder->IsInside(i->m_p2.X+POLECANDIDATE_TOLERANCE_DEPTH,i->m_p2.Y) &&
                   !mFieldLimitsFinder->IsInside(i->m_p1.X,i->m_p1.Y))
        {
            if (fabs(i->m_angle) >= 90.0)
            {
                angle = i->m_angle - 180.0;
            } else {
                angle = i->m_angle;
            }
            while (angle < -180.0) angle += 360.0;
            while (angle > 180.0) angle -= 360.0;
            if ((angle >= 0 && angle <  POLECANDIDATE_ANGLE_MARGIN) ||
                (angle < 0 && angle > -POLECANDIDATE_ANGLE_MARGIN))
            {
                //printf("dentro angle %f\n",angle);
                //mPoleCandidates[mTotalPoleCandidates] = i->m_p2;
                //mTotalPoleCandidates++;
                mPoleCandidates.push_back(i->m_p2);
            } //else {
              //  printf("fora angle %f\n",angle);
              //}
        }
    }
}

void LinesFinder::DrawPoleCandidates(Image *image, int r, int g, int b)
{
    /*for (int idx = 0 ; idx < mTotalPoleCandidates ; idx++)
    {
        int i = (int)mPoleCandidates[idx].X;
        int j = (int)mPoleCandidates[idx].Y;
        VisionUtil::GetInstance()->DrawLine(i-5, j-5, i+5, j+5, r, g, b, image);
        VisionUtil::GetInstance()->DrawLine(i-5, j+5, i+5, j-5, r, g, b, image);
    }*/
    for (std::list<FeaturePoint>::iterator idx = mPoleCandidates.begin() ;
         idx != mPoleCandidates.end() ; idx++)
    {
        int i = (int)idx->X;
        int j = (int)idx->Y;
        
        VisionUtil::GetInstance()->DrawLine(i-5,j-5,i+5,j+5, r,g,b, image);
        VisionUtil::GetInstance()->DrawLine(i-5,j+5,i+5,j-5, r,g,b, image);
    }
}
