/*
 * HistogramList.h
 *
 *  Created on: 04.07.2014
 *      Author: Oliver Krebs
 */

#ifndef HISTOGRAMLIST_H_
#define HISTOGRAMLIST_H_

#include "Histogram.h"
#include <vector>

class HistogramList {
public:
	HistogramList();
	virtual ~HistogramList();

	void clear();
	void add(const Histogram& histogram);

	const vector<Histogram>& get() const;

private:
	std::vector<Histogram> mData;
};

#endif /* HISTOGRAMLIST_H_ */
