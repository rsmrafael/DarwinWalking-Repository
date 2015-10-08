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

/**
 * Representation of a histogram
 */
class Histogram: public Printable {
public:

	/**
	 * Constructor
	 */
	Histogram();

	/**
	 * Destructor
	 */
	virtual ~Histogram();

	/**
	 * set the base paramters
	 * @param minValue		minimum value
	 * @param maxValue		maximum value
	 * @param stepSize		step size
	 */
	void setType(int minValue, int maxValue, int stepSize);

	/**
	 * clear histogram data
	 */
	void clearHist();

	/**
	 * add data to histogram
	 * @param data		data to add
	 */
	void addData(int data);

	/**
	 * get all the data of the histogram
	 * @return map of histogram
	 */
	const std::map<int, int> getHistData() const;

	/**
	 * return minimum bucket label
	 */
	const int getMinBucketLabel() const {
		return mMinBucketLabel;
	}

	/**
	 * return maximum bucket label
	 */
	const int getMaxBucketLabel() const {
		return mMaxBucketLabel;
	}

	/**
	 * return bucket step size
	 */
	const int getBucketStepSize() const {
		return mBucketStepSize;
	}

	/**
	 * return maximum data value
	 */
	const int getMaxDataValue() const {
		return mMaxDataValue;
	}

	/**
	 * calculate and return the mean value of the histogram
	 */
	int calculateMean() const;

	/**
	 * calculate and return the most frequent value
	 */
	int calculateMode() const;

	/**
	 * normalize to byte
	 */
	void normalizeToByte();

	/**
	 * add an entry to histogram
	 * @param key		key
	 * @param value		value
	 */
	void addEntry(int key, int value);

	/**
	 * print to output stream
	 * @param stream 	output stream
	 */
	virtual void print(std::ostream& stream) const {

	}

private:
	std::map<int, int> mData;		//!< data
	int mMinBucketLabel;			//!< min bucket label
	int mMaxBucketLabel;			//!< max bucket label
	int mBucketStepSize;			//!< bucket step size
	int mMaxDataValue;				//!< max data value
};

/**
 * spezialization of Serializer for Histogram
 */
template<>
class Serializer<Histogram> {
public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const Histogram& representation, std::ostream& stream);
	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(std::istream& stream, Histogram& representation);
};

#endif /* Histogramm_H_ */
