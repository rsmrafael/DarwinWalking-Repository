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

/**
 * a printable object
 */
class Printable {
public:

	/**
	 * Default-Constructor
	 */
	Printable() {}

	/**
	 * Copy Constructor
	 */
	Printable(const Printable&) {}

	/**
	 * Destructor
	 */
	virtual ~Printable() {}

	/**
	 * This method must be implemented by a particular
	 * Printable class in order to stream out some specific printable data
	 * @param stream		output stream
	 */
	virtual void print(ostream& stream) const = 0;

	/**
	 * stream object
	 * @param stream	the output stream
	 * @param printable	the printable object to be streamed
	 */
	friend ostream& operator<<(ostream& stream, const Printable& printable) {
	    printable.print(stream);
	    return stream;
	}
};

#endif /* PRINTABLE_H_ */
