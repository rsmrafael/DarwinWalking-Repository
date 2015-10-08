/*
 * DebugDrawer.cpp
 *
 *  Created on: 15.01.2013
 *      Author: Stefan Krupop
 */

#include "DebugDrawer.h"
#include "../Representations/DebugDrawings.h"
#include "../ModuleManager/ModuleManager.h"
#include "../Blackboard/DataHolder.h"

DebugDrawer* DebugDrawer::mInstance = 0;

DebugDrawer* DebugDrawer::GetInstance() {
	if (mInstance == 0) {
		mInstance = new DebugDrawer();
	}
	return mInstance;
}

DebugDrawer::DebugDrawer() {
	if(pthread_mutex_init(&mMutex, NULL)) {
		Debugger::ERR("DebugDrawer", "Could not initialize mutex!");
	}

	DataHolder<DebugDrawings>& rep = ModuleManager::GetInstance()->getBlackboardDirect(true)->getRepresentation<DataHolder<DebugDrawings> >("DebugDrawings");
	mDrawings = &(*rep);
}

DebugDrawer::~DebugDrawer() {
	pthread_mutex_destroy(&mMutex);
	mDrawings = NULL;
}

void DebugDrawer::shutdown() const {
	delete mInstance;
	mInstance = 0;
}

void DebugDrawer::drawObject(std::string module, drawShape type, int x1, int y1, int p1, int p2, drawColor penColor) {
	DebugDrawer* inst = GetInstance();
	pthread_mutex_lock(&(inst->mMutex));
	inst->mDrawings->add(DebugDrawings::DebugDrawing(module, type, x1, y1, p1, p2, penColor));
	pthread_mutex_unlock(&(inst->mMutex));
}

void DebugDrawer::drawText(std::string module, int x, int y, drawColor penColor, std::string text) {
	DebugDrawer* inst = GetInstance();
	pthread_mutex_lock(&(inst->mMutex));
	inst->mDrawings->add(DebugDrawings::DebugText(module, text, x, y, penColor));
	pthread_mutex_unlock(&(inst->mMutex));
}

void DebugDrawer::drawText(std::string module, int x, int y, drawColor penColor, const char *text, ...) {
	va_list args;
	char buffer[512];
	va_start(args, text);
	vsprintf(&buffer[0], text, args);
	va_end(args);
	std::string msg = buffer;
	DebugDrawer::drawText(module, x, y, penColor, msg);
}

void DebugDrawer::newCycle() {
	DebugDrawer* inst = GetInstance();
	pthread_mutex_lock(&(inst->mMutex));
	inst->mDrawings->clear();
	pthread_mutex_unlock(&(inst->mMutex));
}
