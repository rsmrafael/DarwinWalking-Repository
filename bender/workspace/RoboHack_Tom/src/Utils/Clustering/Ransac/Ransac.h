/*
 * Ransac.h
 *
 *  Created on: Feb 22, 2012
 *      Author: Jan Carstensen
 */

#ifndef Ransac_H_
#define Ransac_H_

#include <list>
#include "../../../Representations/Point.h"
#include "Models/AbstractModel.h"


using namespace std;

class Ransac {
public:
	Ransac(const list<Point>* data, AbstractModel* model,
			size_t numberOfIterations, double maximumError);
	~Ransac();

	void init(const list<Point>* data, AbstractModel* model,
			size_t numberOfIterations, double maximumError);

	/**
	 * get the best model represented by the data
	 */
	AbstractModel* run( size_t minFits = 8);

private:
	const list<Point>* mData;
	AbstractModel* mModel;
	size_t mNumberOfIterations;
	double mMaximumError;

};

#endif /* Ransac_H_ */
