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

template<class T>
class BlackboardDataHolder : public BlackboardData {
public:
	BlackboardDataHolder(const std::string& name) : mName(name), mInstance(name) {}
	virtual ~BlackboardDataHolder() {};

    T& operator*() { return mInstance; }

    virtual const std::string getTypeName() const { return typeid(T).name(); }
    virtual const std::string getName() const { return mName; }
    virtual const Representation& getRepresentation() const { return mInstance; }
    virtual Representation& getRepresentation() { return mInstance; }

private:
    std::string mName;
	T mInstance;
};

#endif /* BLACKBOARDDATAHOLDER_H_ */
