/*
 * BlackboardData.h
 *
 *  Created on: 07.08.2012
 *      Author: Stefan Krupop
 */

#ifndef BLACKBOARDDATA_H_
#define BLACKBOARDDATA_H_

#include "Representation.h"
#include <string>

class BlackboardData {
public:
	virtual ~BlackboardData() {}
    virtual const std::string getTypeName() const = 0;
    virtual const std::string getName() const = 0;
    virtual const Representation& getRepresentation() const = 0;
    virtual Representation& getRepresentation() = 0;
};

#endif /* BLACKBOARDDATA_H_ */
