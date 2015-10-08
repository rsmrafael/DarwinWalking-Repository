/*
 * BlackboardDataHolder.h
 *
 *  Created on: 07.08.2012
 *      Author: Stefan Krupop
 */

#ifndef BLACKBOARDDATAHOLDER_H_
#define BLACKBOARDDATAHOLDER_H_

#include "BlackboardData.h"
#include "Representation.h"
#include <string>
#include <typeinfo>

/**
 * template for blackboard data
 */
template<class T>
class BlackboardDataHolder : public BlackboardData {
public:
	/**
	 * Constructor
	 * @param name	the name of the representation
	 */
	BlackboardDataHolder(const std::string& name) : mName(name), mInstance(name) {}

	/**
	 * Destructor
	 */
	virtual ~BlackboardDataHolder() {};

	/**
	 * access to the instance by operator *
	 * @return instance
	 */
    T& operator*() { return mInstance; }

    /**
     * @see BlackboardData::getTypeName
     */
    virtual const std::string getTypeName() const { return typeid(T).name(); }

    /**
	 * @see BlackboardData::getName
	 */
    virtual const std::string getName() const { return mName; }

    /**
	 * @see BlackboardData::getRepresentation
	 */
    virtual const Representation& getRepresentation() const { return mInstance; }

    /**
	 * @see BlackboardData::getRepresentation
	 */
    virtual Representation& getRepresentation() { return mInstance; }

private:
    std::string mName;		//!< the name of the data
	T mInstance;			//!< the representation
};

#endif /* BLACKBOARDDATAHOLDER_H_ */
