/*
 * LocalModeling.h
 *
 *  Created on: 20.02.2013
 *      Author: Jan Carstensen
 *      Edited: Oliver Krebs
 */

#ifndef LocalModeling_H_
#define LocalModeling_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/Odometry.h"
#include "../../Representations/Vector.h"
#include "../../Representations/ObjectList.h"
#include "../../Representations/STLVector.h"
#include "../../Representations/RobotConfiguration.h"
#include "../../Representations/BodyStatus.h"
#include "../../Representations/CameraSettings.h"
#include "../../Representations/Vector.h"
#include "../../ConfigChangeListener.h"
#include <map>

using namespace std;

BEGIN_DECLARE_MODULE(LocalModeling)
	REQUIRE(BodyStatus,BodyStatus)
	REQUIRE(RobotConfiguration,RobotConfiguration)
	REQUIRE(CameraSettings,CameraSettings)
	REQUIRE(Odometry,Odometry)
	REQUIRE(Vector,BallVector)
	REQUIRE(STLVector<Vector>,GoalPolesVectors)
	//----------------------------------------------------
	PROVIDE(Vector,BallRelative)
	PROVIDE(STLVector<Vector>,PolesRelative)
	PROVIDE(ObjectList,LocalModel)
END_DECLARE_MODULE(LocalModeling)

/**
 * LocalModeling creates a local model of all objects (ball, goal poles, robots)
 * TODO does not work correctly yet
 */
class LocalModeling : public LocalModelingBase, public ConfigChangeListener {
public:
	LocalModeling();
	virtual ~LocalModeling();

	/**
	 * execute
	 * @see Module::execute
	 */
	virtual bool execute();

	/**
	 * called when config has changed
	 * @see Module::configChanged
	 */
	virtual void configChanged();

private:
	void updateOdometry(Vector& vec);
	double calculateMatchScoring(Vector &one, Vector &two) const;

	struct CompareData {
		int objectType;
		bool matched;
	};

	std::map<Vector*, CompareData> mLocalModelData;

	ObjectList mLocalModel;		//!<
	double mMaxMatchDistance;	//!<
	double mGain;				//!<
	int mMaxNotSeen;			//!<

	int mOdomX;					//!< current odometry x (mm)
	int mOdomY; 				//!< current odometry y (mm)
	double mOdomOrientation; 	//!< current odometry orientation
	int mLastOdomX; 			//!< last odometry x (mm)
	int mLastOdomY; 			//!< last odometry y (mm)
	double mLastOdomOrientation;//!< last odometry orientation
};

#endif /* LocalModeling_H_ */
