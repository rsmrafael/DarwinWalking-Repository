/*
 * BlackboardExceptions.h
 *
 *  Created on: 07.08.2012
 *      Author: Stefan Krupop
 */

#ifndef BLACKBOARDEXCEPTIONS_H_
#define BLACKBOARDEXCEPTIONS_H_

#include <string>
#include <stdexcept>

/**
 * Exception for a representation type mismatch
 */
class RepresentationTypeMismatch : public std::invalid_argument {
public:
	/**
	 * Constructor
	 * @param __arg		argument
	 */
	explicit RepresentationTypeMismatch(const std::string& __arg) : std::invalid_argument(__arg) {};

	/**
	 * Destructor
	 */
	virtual ~RepresentationTypeMismatch() throw () {};
};

/**
 * Exception for a representation that is not existing
 */
class RepresentationNotExisting : public std::invalid_argument {
public:
	/**
	 * Constructor
	 * @param __arg		argument
	 */
	explicit RepresentationNotExisting(const std::string& __arg) : std::invalid_argument(__arg) {};

	/**
	 * Destructor
	 */
	virtual ~RepresentationNotExisting() throw () {};
};

#endif /* BLACKBOARDEXCEPTIONS_H_ */
