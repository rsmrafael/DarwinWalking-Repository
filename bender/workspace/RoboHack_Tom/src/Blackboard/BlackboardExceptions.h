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

class RepresentationTypeMismatch : public std::invalid_argument {
public:
	explicit RepresentationTypeMismatch(const std::string& __arg) : std::invalid_argument(__arg) {};
	virtual ~RepresentationTypeMismatch() throw () {};
};

class RepresentationNotExisting : public std::invalid_argument {
public:
	explicit RepresentationNotExisting(const std::string& __arg) : std::invalid_argument(__arg) {};
	virtual ~RepresentationNotExisting() throw () {};
};

#endif /* BLACKBOARDEXCEPTIONS_H_ */
