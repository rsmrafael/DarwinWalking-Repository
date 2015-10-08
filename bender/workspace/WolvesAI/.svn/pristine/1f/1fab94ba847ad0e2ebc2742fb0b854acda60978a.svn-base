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

/**
 * Ransac algorithm generates random models and returns the best fitting model
 */
class Ransac {
public:

	/**
	 * Constructor
	 * @param data					given point cloud
	 * @param model					given model
	 * @param numberOfIterations	number of created and compared models
	 * @param maximumError			maximal error for points
	 */
	Ransac(const list<Point>* data, AbstractModel* model,
			size_t numberOfIterations, double maximumError);

	/**
	 * Destructor
	 */
	~Ransac();

	/**
	 * initialize
	 * @param data					given point cloud
	 * @param model					given model
	 * @param numberOfIterations	number of created and compared models
	 * @param maximumError			maximal error for points
	 */
	void init(const list<Point>* data, AbstractModel* model,
			size_t numberOfIterations, double maximumError);

	/**
	 * set number of iterations
	 * @param number 			number of created and compared models
	 */
	void setNumberOfIterations( size_t number);

	/**
	 * get the best model represented by the data
	 * @param minFits		minimal fits of points
	 * @return best fitting model
	 */
	AbstractModel* run( size_t minFits = 8);

private:
	const list<Point>* mData;
	AbstractModel* mModel;
	size_t mNumberOfIterations;
	double mMaximumError;

};

#endif /* Ransac_H_ */
