/*
 * Histogramm.cpp
 *
 *  Created on: Sep 24, 2012
 *      Author: jan
 */

#include "Histogram.h"
#include "../Messages/Line.pb.h"
#include "../Messages/Histogram.pb.h"
#include "../Messages/OstreamOutputStream.h"
#include "../Messages/IstreamInputStream.h"
#include <algorithm>
#include <limits>

using namespace std;

Histogram::Histogram() :
		mMinBucketLabel(0),
		mMaxBucketLabel(255),
		mBucketStepSize(16),
		mMaxDataValue(0) {
	clearHist();
}

Histogram::~Histogram() {
	clearHist();
}

void Histogram::clearHist() {
	mData.clear();
	mMaxDataValue = 0;

	for (int i = mMinBucketLabel; i <= mMaxBucketLabel; i += mBucketStepSize) {
		addEntry(i, 0);
	}
}

void Histogram::setType(int minValue, int maxValue, int stepSize) {
	mMinBucketLabel = minValue;
	mMaxBucketLabel = maxValue;
	mBucketStepSize = stepSize;

	clearHist();
}

void Histogram::addData(int data) {

	if (data >= mMinBucketLabel && data <= mMaxBucketLabel) {

		// round the data to a stepValue (integer division!)
		int roundedData = data / mBucketStepSize;
		roundedData *= mBucketStepSize;

		int value = 0;
		map<int, int>::iterator it;
		it = mData.find(roundedData);

		if (it != mData.end()) {
			value = it->second;
			//mData.erase(it);
		}

		++value;
		addEntry(roundedData, value);
	}
}

const map<int, int> Histogram::getHistData() const {
	return mData;
}

void Histogram::normalizeToByte() {
	static const int normalizedMax = std::numeric_limits<uint8_t>::max();
	//Catch division by zero
	if (mMaxDataValue == 0) {
		return;
	}
	//Normalize to UINT8_MAX (should be 255)
	for (map<int, int>::iterator it = mData.begin(); it != mData.end(); ++it) {
		(*it).second = static_cast<int>(((*it).second) / ((float) mMaxDataValue
				* normalizedMax));
	}
	mMaxDataValue = normalizedMax;
}

void Histogram::addEntry(int key, int value) {
	mMaxDataValue = max(value, mMaxDataValue);
	mData[key] = value;
}

int Histogram::calculateMean() const {
	int mean = 0;
	int counter = 1;
	for (map<int, int>::const_iterator it = mData.begin(); it != mData.end();
			++it) {
		counter += it->second;
		mean -= (mean - it->first) / counter;
	}
	return mean;
}

int Histogram::calculateMode() const {
	int mode = 0;
	int maximum = 0;
	for (map<int, int>::const_iterator it = mData.begin(); it != mData.end();
			++it) {
		if (it->second > maximum) {
			maximum = it->second;
			mode = it->first;
		}
	}
	return mode;
}

void Serializer<Histogram>::serialize(const Histogram& representation,
		ostream& stream) {

	protobuf::Histogram histogram;

	const map<int, int> data = representation.getHistData();

	for (map<int, int>::const_iterator mapIterator = data.begin();
			mapIterator != data.end(); ++mapIterator) {
		histogram.add_value((*mapIterator).second);
	}

	histogram.set_maxvalue(representation.getMaxBucketLabel());
	histogram.set_minvalue(representation.getMinBucketLabel());
	histogram.set_stepsize(representation.getBucketStepSize());

	OstreamOutputStream buf(&stream);
	histogram.SerializeToZeroCopyStream(&buf);
}

void Serializer<Histogram>::deserialize(istream& stream,
		Histogram& representation) {
	protobuf::Histogram hist;

	IstreamInputStream buf(&stream);
	hist.ParseFromZeroCopyStream(&buf);

	representation.addData(0);
}
