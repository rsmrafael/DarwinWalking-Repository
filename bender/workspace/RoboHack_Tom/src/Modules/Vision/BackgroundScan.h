/*
 * BackgroundScan.h
 *
 *  Created on: 21.03.2014
 *      Author: Oliver Krebs
 */

#ifndef BACKGROUNDSCAN_H_
#define BACKGROUNDSCAN_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/ObjectList.h"
#include "../../Representations/FieldLines.h"
#include "../../Representations/Histogram.h"
#include "../../ConfigChangeListener.h"

BEGIN_DECLARE_MODULE(BackgroundScan)
	REQUIRE(YUVImage,Image)
	REQUIRE(FieldLines,FieldLines)
	REQUIRE(ObjectList, GoalPoles)
	PROVIDE(Histogram,BackgroundY)
	PROVIDE(Histogram,BackgroundU)
	PROVIDE(Histogram,BackgroundV)
	PROVIDE(Histogram,BackgroundYUV)
END_DECLARE_MODULE(BackgroundScan)

class BackgroundScan
:	public BackgroundScanBase,
	public ConfigChangeListener
{
public:
	BackgroundScan();
	virtual ~BackgroundScan();

	virtual bool execute();
	virtual void configChanged();

private:
	int mMaxU_Goal;
	int mMaxVaddU_Goal;
	bool mScanBackground;
};


#endif /* BACKGROUNDSCAN_H_ */