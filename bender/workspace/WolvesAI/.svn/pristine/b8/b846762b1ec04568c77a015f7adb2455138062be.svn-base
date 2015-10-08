/*
 * Ransac.cpp
 *
 *  Created on: Feb 22, 2012
 *      Author: Jan Carstensen
 */

#include "Ransac.h"
#include <stdio.h>
#include "Models/AbstractModel.h"

Ransac::Ransac(const list<Point>* data, AbstractModel* model,
		size_t numberOfIterations, double maximumError)
:	mData( data),
 	mModel( model),
 	mNumberOfIterations( numberOfIterations),
 	mMaximumError( maximumError)
{

}

Ransac::~Ransac() {
	mData = NULL;
	delete mModel;
}

void Ransac::init(const list<Point>* data, AbstractModel* model,
		size_t numberOfIterations, double maximumError) {
	mData = data;
	mMaximumError = maximumError;
	mNumberOfIterations = numberOfIterations;
	mModel = model;
}

AbstractModel* Ransac::run(size_t minFits) {

	AbstractModel* bestModel = NULL;
	AbstractModel* currentModel;

	size_t currentFits;
	size_t size = mData->size();
	if( size < minFits) {
		return NULL;
	}

	for (size_t iteration = 0; iteration < mNumberOfIterations; ++iteration) {

		currentModel = mModel->createNextModel(mData);
		if (currentModel == NULL) {
			break;
		}
		currentFits = currentModel->getNumberOfFittedDataPoints(mData,
				mMaximumError);
		if (currentFits > minFits) {
			delete bestModel;
			bestModel = currentModel;
			minFits = currentFits;
		} /*else if (currentFits == minFits && bestModel != NULL) {
			if (currentModel->calculateError(mData)
					< bestModel->calculateError(mData)) {
				delete bestModel;
				bestModel = currentModel;
				maxFits = currentFits;
			} else {
				delete currentModel;
			}
		} */else {
			delete currentModel;
		}

	}

	//Debugger::DEBUG("Ransac", "Fits: %i", minFits);
	return bestModel;
}

