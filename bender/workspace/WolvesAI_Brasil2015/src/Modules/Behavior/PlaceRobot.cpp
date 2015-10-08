/*
 * PlaceRobot.cpp
 *
 *  Created on: 05.02.2015
 *      Author: bombadil
 */

#include "PlaceRobot.h"

PlaceRobot::PlaceRobot(Game *game, MitecomCommunicator *communicator)
:	AbstractBehavior( game, communicator){
	mThisRole = ROLE_PLACE_ROBOT;
	often = 0;
	lostCounterGoal=0;
	mCurrentState = MOVE_TO_HALF;
	entryMoveHalf();
}

PlaceRobot::~PlaceRobot() {
	// TODO Auto-generated destructor stub
}

void PlaceRobot::executeBehavior(){
	//Debugger::DEBUG("PlaceRobot", "RUN");
	switch(mCurrentState) {
	case MOVE_TO_HALF:
		mCurrentState = processMoveHalf();
		break;
	case ROTATE_TO_GOAL:
		mCurrentState = processRotateToGoal();
		break;
	case MOVE_TO_GOAL:
		mCurrentState = processMoveToGoal();
		break;
	case DELAY_LOOP:
		mCurrentState = processDelayLoop();
		break;
	default:
		Debugger::INFO("PlaceRobot", "Unknown state, change to moveToFieldLine");
		mCurrentState = MOVE_TO_HALF;
		break;
	}
}
void PlaceRobot::updateState(){

	if (mCurrentState != mLastState) {
		Debugger::announce(getStateName(mCurrentState));
		switch( mCurrentState) {
		case MOVE_TO_HALF:
			entryMoveHalf();
			break;
		case ROTATE_TO_GOAL:
			entryRotateToGoal();
			break;
		case MOVE_TO_GOAL:
			entryMoveToGoal();
			break;
		default:
			mMyInfo.data[ROBOT_CURRENT_ACTION] = ACTION_WAITING;
			//Debugger::announce("Waiting");
			break;
		}
		Debugger::INFO("PlaceRobot", "Changed state %d to %d", mLastState, mCurrentState);
		mLastState = mCurrentState;
	}
}
void PlaceRobot::entryMoveHalf(){

	getHeadAction().searchObject(Object::BALL, true);
#ifdef ROBOTAUDIO
	//RobotAudio::BIER();
#endif

}
int PlaceRobot::processMoveHalf(){
	VisionStatus::ObjectStatus status = getGoalStatus().getObjectStatus();
	getBodyAction().setWalkerCommand(0, 0, 0);
	getBodyAction().stop();
	vector<Object> goalPoles=getGoalPoles().getObjects();
	Debugger::DEBUG("PlaceRobot","GoalPoolles %d",goalPoles.size());
	if(goalPoles.size()==2){
		double d =(double)goalPoles.at(0).lastImageHeight/(double)goalPoles.at(1).lastImageHeight;
		Debugger::DEBUG("PlaceRobot","VALUE=%f",d);
	}
	switch( status) {
	case VisionStatus::OBJECT_FOUND_STABLE:
		if(goalPoles.size()>=2){
					double d =(double)goalPoles.at(0).lastImageHeight/(double)goalPoles.at(1).lastImageHeight;
					Debugger::DEBUG("PlaceRobot","VALUE=%f",d);
					if(d<1.1&&d>0.90){
						if(often>4){
							Debugger::INFO("PlaceRobot", "Rotate");
							return ROTATE_TO_GOAL;
						}else{
							Debugger::INFO("PlaceRobot", "GoalOften: %d", often);
							often++;
						}

					}else{
						often = 0;
						getBodyAction().setWalkerCommand(100, 0, 0);
						initDelayLoop(1000L,MOVE_TO_HALF);
						return DELAY_LOOP;
					}
				}
		break;
	case VisionStatus::OBJECT_SEARCHING:
	case VisionStatus::OBJECT_NOT_FOUND:
	case VisionStatus::OBJECT_LOST:
	default:
		break;
	}


	//Debugger::INFO("PlaceRobot", "GoalStatus: %d", status);
	/*getBodyAction().setWalkerCommand(100, 0, 0);
	vector<Object> goalPoles=getGoalPoles().getObjects();
	if(goalPoles.size()>=2){
		double d =(double)goalPoles.at(0).lastImageHeight/(double)goalPoles.at(1).lastImageHeight;
		Debugger::DEBUG("PlaceRobot","VALUE=%f",d);
		if(d<1.1&&d>0.90){
			if(often>4){
				Debugger::INFO("PlaceRobot", "Rotate");
				return ROTATE_TO_GOAL;
			}else{
				Debugger::INFO("PlaceRobot", "GoalOften: %d", often);
				often++;
			}

		}else{
			often = 0;
		}
	}*/
	return MOVE_TO_HALF;
}


void PlaceRobot::entryRotateToGoal(){
	getHeadAction().searchObject(Object::GOAL_YELLOW_CROSSBAR, true);
}
int PlaceRobot::processRotateToGoal(){
	getBodyAction().circleLeft();
	//getBodyAction().stop();
	initDelayLoop(5000L,MOVE_TO_GOAL);
	Debugger::DEBUG("PlaceRobot","RotateStop");
	return DELAY_LOOP;
}

void PlaceRobot::entryMoveToGoal(){
	getHeadAction().searchObject(Object::GOAL_YELLOW_CROSSBAR, false);
}
int PlaceRobot::processMoveToGoal(){
	VisionStatus::ObjectStatus status = getGoalStatus().getObjectStatus();
	int pan = getBodyStatus().getPan();
	int tilt = getBodyStatus().getTilt();
	switch( status) {
	case VisionStatus::OBJECT_FOUND_STABLE:
		if(pan > -10 && pan < 10) {
			int16_t fbStep, rlStep;
			calculateWalkSteps( pan, tilt, rlStep, fbStep);
			getBodyAction().setWalkerCommand(fbStep, 0, rlStep);
			Debugger::DEBUG("PlaceRobot", "forward %d, rotating %d", fbStep, rlStep);
		} else {
			int16_t rlStep;
			int16_t yStep;
			calculateTurnWalkSteps( pan, rlStep, yStep);
			getBodyAction().setWalkerCommand(-100, yStep, 100);
			Debugger::DEBUG("PlaceRobot", "rotating %d", rlStep);
		}
		lostCounterGoal = 0;
		break;
	case VisionStatus::OBJECT_SEARCHING:
		getBodyAction().stop();
		break;
	case VisionStatus::OBJECT_NOT_FOUND:
	case VisionStatus::OBJECT_LOST:
		lostCounterGoal++;
		getBodyAction().stop();
		if(lostCounterGoal>20){
			getBodyAction().kickLeft();
			initDelayLoop(5000L,MOVE_TO_GOAL);
			return DELAY_LOOP;

		}
		break;
	default:
		break;
	}
	/*if(getFieldLines().getEdgePoints()->size()>1){

		//CHECK IF ROBOT WALKS STRAIGHT
		OwnPoint lastPoint = OwnPoint(-10,-10);
		vector<vector<OwnPoint> > points;
		for (list<OwnPoint>::const_iterator pointIterator =getFieldLines().getEdgePoints()->begin();
				pointIterator != getFieldLines().getEdgePoints()->end(); ++pointIterator) {

			OwnPoint p = OwnPoint(pointIterator->getX(),pointIterator->getY());
			if(lastPoint.getX()!=-10){
				vector<OwnPoint> newLine;
				newLine.push_back(p);
				newLine.push_back(lastPoint);
				points.push_back(newLine);
			}
			lastPoint = p;
			Debugger::DEBUG("PlaceRobot","LastPoint is %i,%i",lastPoint.getX(),lastPoint.getY());
		}
		Debugger::DEBUG("PlaceRobot","ListedPoints");
		//Find Largest Line
		double lineLength = 0.0;
		int indexLarge = -1;
		for(int i=0;i<points.size();i++){
			double diffx = abs(points.at(i)[0].getX()-points.at(i)[1].getX());
			double diffy = abs(points.at(i)[0].getY()-points.at(i)[1].getY());
			double lange = sqrt(diffx*diffx+diffy*diffy);
			if(lange>lineLength){
				indexLarge = i;
				lineLength = lange;
			}
		}
		Debugger::DEBUG("PlaceRobot","FindLargestLine");
		if(indexLarge!=-1&&points.at(indexLarge)[0].getY()<=10){
			double steigung = (double)(points.at(indexLarge).at(0).getY() -points.at(indexLarge).at(1).getY())/(double)(points.at(indexLarge).at(0).getX()-points.at(indexLarge).at(1).getX());
			Debugger::DEBUG("PlaceRobot","Steigung ist: %f",steigung);
		}
		points.clear();

	}*/



	return MOVE_TO_GOAL;
}
string PlaceRobot::getStateName(int state){
	switch(state){
	case MOVE_TO_HALF: return "MOVE_TO_HALF";
	case MOVE_TO_GOAL: return "MOVE_TO_GOAL";
	case ROTATE_TO_GOAL: return "ROTATE_TO_GOAL";
	default: return AbstractBehavior::getStateName(state);
	}
}
