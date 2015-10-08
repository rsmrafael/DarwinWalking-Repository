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

/**
 * data of blackboard
 */
class BlackboardData {
public:
	/**
	 * Destructor
	 */
	virtual ~BlackboardData() {}

	/**
	 * get the name of data type
	 * @return type name
	 */
    virtual const std::string getTypeName() const = 0;

    /**
     * get the name of the data
     * @return name
     */

    virtual const std::string getName() const = 0;

    /**
     * get the representation
     * @return representation
     */
    virtual const Representation& getRepresentation() const = 0;

    /**
     * get the representation
     * @return representation
     */
    virtual Representation& getRepresentation() = 0;
};

#endif /* BLACKBOARDDATA_H_ */
