/*
 * HeadMovement.h
 *
 *  Created on: 24.04.2014
 *      Author: Oliver Krebs
 */

#ifndef HEADMOVEMENT_H_
#define HEADMOVEMENT_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/Object.h"
#include "../../Representations/ObjectList.h"
#include "../../Representations/RobotConfiguration.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/HeadAction.h"
#include "../../Representations/VisionStatus.h"
#include "../../Representations/BehaviorStatus.h"
#include "../../Representations/FieldLines.h"

BEGIN_DECLARE_MODULE(HeadMovement)
	REQUIRE(Object,BallPredict)
	REQUIRE(ObjectList,GoalPoles)
	REQUIRE(Object, BallRelative)
	PROVIDE(ObjectList,PolesRelative)
	REQUIRE(RobotConfiguration,RobotConfiguration)
	REQUIRE(YUVImage, Image)
	REQUIRE(BehaviorStatus,BehaviorStatus)
	REQUIRE(FieldLines, FieldLines)
	PROVIDE(HeadAction, HeadAction)
	PROVIDE(VisionStatus, BallStatus)
	PROVIDE(VisionStatus, GoalStatus)
END_DECLARE_MODULE(HeadMovement)

class HeadMovement : public HeadMovementBase {
public:
	HeadMovement();
	virtual ~HeadMovement();

	bool execute();

private:
	bool searchObject(int minRow, int maxRow);
	bool processTracker(const Object& object) const;
	void searchBall();
	void searchGoal();
	void searchFieldLine();
	int getLineY();
	void resetSearch();

	int mSearchObjectRow;
	int mSearchObjectPan;
	int mSearchObjectTilt;
	int mSearchObjectPanDir;
	int mSearchObjectTiltDir;
	int mSearchObjectLoopPoint;
	int mSearchObjectUpdateCnt;

	int mFoundCounter;
	int mNotFoundCounter;
	int mLostCounter;

	Object::ObjectType mLastSearchObjectType;
};

#endif /* HEADMOVEMENT_H_ */