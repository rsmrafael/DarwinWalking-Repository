/*
 * STLVector.h
 *
 *  Created on: 01.07.2014
 *      Author: gabel
 */

#ifndef STLVECTOR_H_
#define STLVECTOR_H_

#include <vector>

/**
 * Represents a STL-Vector from std::vector of the generic type T
 * as possible blackboard object. Needed because std::vector is not polymorphic,
 * which is needed by the blackboard representation data holder.
 */
template<class T>
class STLVector {
public:
	STLVector() {
	}

	// lint -e(1509) base class destructor for class 'vector' is not virtual
	virtual ~STLVector() {
	}

	inline const std::vector<T>& get() const {
		return data;
	}

	std::vector<T> data;
};

#endif /* STLVECTOR_H_ */
