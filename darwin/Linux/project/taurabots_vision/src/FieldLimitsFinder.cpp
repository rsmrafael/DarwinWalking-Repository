#include "FieldLimitsFinder.h"
#include "VisionUtil.h"

#define LIMIT_COL2INDEX(i) (((i)-FIELDLIMITS_MARGIN+(FIELDLIMITS_STEP/2)) / FIELDLIMITS_STEP)
#define LIMIT_INDEX2COL(i) (FIELDLIMITS_MARGIN + (i)*FIELDLIMITS_STEP)

FieldLimitsFinder::FieldLimitsFinder(Image *image)
{
    mFieldColorMap = 0;
    mMaxIndex = (image->m_Width-2*FIELDLIMITS_MARGIN)/FIELDLIMITS_STEP;
    mMinLimits = new int[mMaxIndex];
    mMaxLimits = new int[mMaxIndex];
}

FieldLimitsFinder::~FieldLimitsFinder()
{
    delete[] mMinLimits;
    delete[] mMaxLimits;
}

void FieldLimitsFinder::FindLimits(Image *image)
{
    int j_index = 0;
    for (int j = FIELDLIMITS_MARGIN ; j < image->m_Width-FIELDLIMITS_MARGIN ; j += FIELDLIMITS_STEP)
    {
        mMinLimits[j_index] = -1;
        mMaxLimits[j_index] = -1;
        for (int i = FIELDLIMITS_MARGIN ; i < image->m_Height-FIELDLIMITS_MARGIN; i += FIELDLIMITS_STEP)
        {
            if (is_pixel_in_colormap(*((rgb_pixel_t *)(image->m_ImageData +
                                ((i * image->m_Width) + j)*3)), mFieldColorMap))
            {
                mMinLimits[j_index] = i;
                break;
            }
        }
        for (int i = image->m_Height-FIELDLIMITS_MARGIN ; i > FIELDLIMITS_MARGIN ; i -= FIELDLIMITS_STEP)
        {
            if (is_pixel_in_colormap(*((rgb_pixel_t *)(image->m_ImageData +
                                ((i * image->m_Width) + j)*3)), mFieldColorMap))
            {
                mMaxLimits[j_index] = i;
                break;
            }
        }
        j_index++;
    }
}

bool FieldLimitsFinder::IsInside(int x, int y)
{
    int j_index = LIMIT_COL2INDEX(y);
    if (j_index < 0) j_index = 0;
    if (x >= mMinLimits[j_index] && x <= mMaxLimits[j_index])
    {
        return true;
    } else {
        if (j_index > 0)
        {
            if (x >= mMinLimits[j_index-1] && x <= mMaxLimits[j_index-1])
            {
                return true;
            }
        } else if (j_index < mMaxIndex-1)
        {
            if (x >= mMinLimits[j_index+1] && x <= mMaxLimits[j_index+1])
            {
                return true;
            }
        }
        return false;
    }
}

void FieldLimitsFinder::DrawLimits(Image *image)
{
    for (int i = 0 ; i < mMaxIndex-1 ; i++)
    {
        if (mMaxLimits[i] != -1 && mMaxLimits[i+1] != -1)
        {
            VisionUtil::GetInstance()->DrawLine(mMaxLimits[i],LIMIT_INDEX2COL(i),
                                                mMaxLimits[i+1],LIMIT_INDEX2COL(i+1),
                                                255,0,255,image);
        }
        if (mMinLimits[i] != -1 && mMinLimits[i+1] != -1)
        {
            VisionUtil::GetInstance()->DrawLine(mMinLimits[i],LIMIT_INDEX2COL(i),
                                                mMinLimits[i+1],LIMIT_INDEX2COL(i+1),
                                                255,0,255,image);
        }
        if (mMinLimits[i] != -1 && mMaxLimits[i] != -1)
        {
            VisionUtil::GetInstance()->DrawLine(mMinLimits[i],LIMIT_INDEX2COL(i),
                                                mMaxLimits[i],LIMIT_INDEX2COL(i),
                                                0,255,255,image);
        }
    }
    VisionUtil::GetInstance()->DrawLine(mMinLimits[mMaxIndex-1],
                                        LIMIT_INDEX2COL(mMaxIndex-1),
                                        mMaxLimits[mMaxIndex-1],
                                        LIMIT_INDEX2COL(mMaxIndex-1),
                                        0,255,255,image);
}

