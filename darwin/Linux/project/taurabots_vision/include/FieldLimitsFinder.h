#ifndef _FIELDLIMITS_H_
#define _FIELDLIMITS_H_

#include <stdint.h>
#include "cblob.h"
#include "Image.h"

#define FIELDLIMITS_STEP 20
#define FIELDLIMITS_MARGIN 20

using namespace Robot;

class FieldLimitsFinder {
    public:
        FieldLimitsFinder(Image *image);
        ~FieldLimitsFinder();
        void SetFieldColorMap(uint16_t *colorMap) { mFieldColorMap = colorMap; }
        uint16_t *GetFieldColorMap() { return mFieldColorMap; }
        void FindLimits(Image *image);
        bool IsInside(int x, int y);
        void DrawLimits(Image *image);
    private:
        uint16_t *mFieldColorMap;
        int *mMaxLimits;
        int *mMinLimits;
        int mMaxIndex;
};

#endif

