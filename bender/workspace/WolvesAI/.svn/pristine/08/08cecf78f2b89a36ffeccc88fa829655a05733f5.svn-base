/*
 * Printable.h
 *
 *  Created on: 08.08.2012
 *      Author: Stefan Krupop
 */

#ifndef PRINTABLE_H_
#define PRINTABLE_H_

#include <iostream>

using namespace std;

class Printable {
public:

	Printable() {}
	virtual ~Printable() {}

	Printable(const Printable&) {}

	/**
	 * This method must be overwritten by a particular
	 * Printable in order to stream out some specific (visible) data
	 */
	virtual void print(ostream& stream) const = 0;

	friend ostream& operator<<(ostream& stream, const Printable& printable) {
	    printable.print(stream);
	    return stream;
	}
};

#endif /* PRINTABLE_H_ */
