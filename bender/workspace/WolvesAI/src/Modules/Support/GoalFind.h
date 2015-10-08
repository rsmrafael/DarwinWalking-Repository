/*
 * GoalFind.h
 *
 *  Created on: 16.03.2015
 *      Author: bombadil
 */

#ifndef GOALFIND_H_
#define GOALFIND_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/FieldLines.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/ObjectList.h"
#include "../../ConfigChangeListener.h"
#include <cmath>
#include <algorithm>


BEGIN_DECLARE_MODULE(GoalFind)
	REQUIRE(FieldLines,FieldLines)
	REQUIRE(YUVImage,Image)
	PROVIDE(ObjectList, GoalPoles)
END_DECLARE_MODULE(GoalFind)

/**
 * Goal Find
 */
class GoalFind : public GoalFindBase, public ConfigChangeListener{
public:
	/**
	 * Constructor
	 */
	GoalFind();

	/**
	 * Destructor
	 */
	virtual ~GoalFind();

	/**
	 * execute
	 */
	virtual bool execute();

	/**
	 * config changed
	 */
	virtual void configChanged();

private:
	int mDebugDrawings;				//!< debug drawings
	int mMaxU_Green;				//!< max u green
	int mMaxV_Green;				//!< max v green
	int mMinY_Whitegoal;			//!< min y whitegoal
	int mMinPoleWidth;				//!< min pole width
	int mMinPoleHeight;				//!< min pole height
	int mPoleWatchBelowLine;		//!< pole watch below line

	/**
	 * find max height
	 */
	int findMaxHeight(int startX,int startY);

	/**
	 * compare poles
	 */
	bool poleCompare(const pair<int, Object>& firstElem, const std::pair<int, Object>& secondElem);
};

#endif /* GOALFIND_H_ */
