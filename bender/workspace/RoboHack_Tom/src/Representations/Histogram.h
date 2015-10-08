/*
 * Histogramm.h
 *
 *  Created on: Sep 24, 2012
 *      Author: jan
 */

#ifndef Histogramm_H_
#define Histogramm_H_

#include "../Blackboard/Printable.h"
#include "../Blackboard/Serializer.h"
#include <vector>
#include <list>
#include <map>

class Histogram: public Printable {
public:
	Histogram();
	virtual ~Histogram();

	void setType(int minValue, int maxValue, int stepSize);
	void clearHist();
	void addData(int data);

	const std::map<int, int> getHistData() const;
	const int getMinBucketLabel() const {
		return mMinBucketLabel;
	}
	const int getMaxBucketLabel() const {
		return mMaxBucketLabel;
	}
	const int getBucketStepSize() const {
		return mBucketStepSize;
	}

	const int getMaxDataValue() const {
		return mMaxDataValue;
	}

	int calculateMean() const;
	// mode (deutsch modus/modalwert): most frequent value
	int calculateMode() const;

	void normalizeToByte();

	void addEntry(int key, int value);

	virtual void print(std::ostream& stream) const {

	}

private:
	std::map<int, int> mData;
	int mMinBucketLabel;
	int mMaxBucketLabel;
	int mBucketStepSize;
	int mMaxDataValue;
};

template<>
class Serializer<Histogram> {
public:
	static void serialize(const Histogram& representation, std::ostream& stream);
	static void deserialize(std::istream& stream, Histogram& representation);
};

#endif /* Histogramm_H_ */
