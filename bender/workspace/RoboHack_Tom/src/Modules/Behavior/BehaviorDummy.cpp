/*
 * BehaviorDummy.cpp
 *
 *  Created on: 20.10.2014
 *      Author: bombadil
 */
#include "BehaviorDummy.h"

BehaviorDummy::BehaviorDummy(){
	cycle = 0;
	state = 0;
}

BehaviorDummy::~BehaviorDummy(){

}

bool BehaviorDummy::execute(){
	aufgabe5();

	incrementCycle();
	return true;
}

void BehaviorDummy::incrementCycle() {
	cycle++;
}

void BehaviorDummy::aufgabe1() {
	Debugger::INFO("BehaviorDummy", "Zyklus: %d", cycle);
}


void BehaviorDummy::aufgabe2() {
	int pan = getBodyStatus().getPan();
	int tilt = getBodyStatus().getTilt();
	Debugger::INFO("BehaviorDummy", "Pan: %d", pan);
	Debugger::INFO("BehaviorDummy", "Tilt: %d",  tilt);
}

void BehaviorDummy::aufgabe3() {
	if ((cycle % 100 == 0) && cycle != 0) {
		Debugger::INFO("BehaviorDummy", "Shoot! C: %d", cycle);
		getBodyAction().kickRight();
	} else {
		Debugger::INFO("BehaviorDummy", "Walk. C: %d", cycle);
		getBodyAction().setWalkerCommand(20,0,0);
	}
}

void BehaviorDummy::aufgabe4() {
	if (cycle == 1) {
		getHeadAction().searchObject(Object::BALL, true);
	} else {
		VisionStatus::ObjectStatus status = getBallStatus().getObjectStatus();
		switch (status) {
			case VisionStatus::OBJECT_FOUND_STABLE:
				break;

			case VisionStatus::OBJECT_LOST:
				break;

			case VisionStatus::OBJECT_NOT_FOUND:
				break;

			case VisionStatus::OBJECT_SEARCHING:
				break;

			case VisionStatus::OBJECT_SEEN:
				Debugger::INFO("BehaviorDummy", "Ball Seen");
				break;

			default:
				break;
		}
	}
}

void BehaviorDummy::aufgabe5() {
	int pan;
	int tilt;
	int fbStep;
	int rlStep;
	Debugger::INFO("BehaviorDummy", "Beginning State %d", state);
	if (cycle == 0) {
		getHeadAction().searchObject(Object::BALL, true);
	}

	switch (state) {
		case 0:
			getBodyAction().stop();
			getHeadAction().searchObject(Object::BALL, false);
			if (getBallStatus().getObjectStatus() == VisionStatus::OBJECT_FOUND_STABLE) {
				state = 1;
			}
			break;

		case 1:
			if (getBallStatus().getObjectStatus() != VisionStatus::OBJECT_FOUND_STABLE) {
				state = 0;
			} else {
				if ((int)getBall().lastVector.getLength() < 125) {
					state = 3;
				} else {
					pan = getBodyStatus().getPan();
					tilt = getBodyStatus().getTilt();
					fbStep = (int16_t)((double)tilt)/6.0;
					rlStep = (int16_t)(double)pan/3;
					Debugger::INFO("BehaviorDummy", "fbStep: %d", fbStep);
					Debugger::INFO("BehaviorDummy", "rlStep: %d",  rlStep);

					//Skalierung der Werte
					if (fbStep < 5) {
						fbStep = 10;
					} else {
						fbStep = 20;
					}

					if (rlStep < -20) {
						rlStep = -70;
						fbStep = -3;
					} else if ((rlStep > -20 && rlStep < -10) || (rlStep > 20 && rlStep > 10)) {
						//nothing to do here
						rlStep *= 2;
					} else if (rlStep > -10 && rlStep < 10) {
						rlStep *= 2;
						fbStep *= 2;
					} else if (rlStep > 20) {
						rlStep = 70;
						fbStep = -3;
					} else {
						Debugger::INFO("BehaviorDummy", "Unknown Else State-HOW");
					}
					getBodyAction().stop();
					getBodyAction().setWalkerCommand(fbStep, 0, rlStep);
				}
			}
			break;

		case 2:
//			kick distance 125
			pan = getBodyStatus().getPan();
			rlStep = (int16_t)(double)pan/3;
			Debugger::INFO("BehaviorDummy", "Kick: %d", fbStep);
			if (rlStep < 0) {
				getBodyAction().kickLeft();
			} else {
				getBodyAction().kickRight();
			}
			state = 0;
			break;

		case 3:
			//Distance 125
			//TODO einwurfmotion
			getBodyAction().playMotion(ICommunicator::RaiseLeftHand);
			state = 0;
			break;

		default:
			Debugger::INFO("BehaviorDummy", "Unknown State! State: %d", state);
			break;
	}
	Debugger::INFO("BehaviorDummy", "End State %d", state);
}
