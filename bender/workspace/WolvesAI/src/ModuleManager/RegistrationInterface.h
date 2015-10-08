/*
 * RegistrationInterface.h
 *
 *  Created on: 28.08.2012
 *      Author: Stefan Krupop
 */

#ifndef REGISTRATIONINTERFACE_H_
#define REGISTRATIONINTERFACE_H_

#include "../Blackboard/Representation.h"
#include "../Blackboard/DataHolder.h"
#include "ModuleManager.h"
#include <map>

using namespace std;

/**
 * Interface for a registration
 */
class RegistrationInterface {
public:
	virtual ~RegistrationInterface() {}

	/**
	 * register at blackboard
	 */
	virtual Representation& registerAtBlackBoard() = 0;
};

// type for a named map of representations
typedef map<string, RegistrationInterface*> RepresentationMap;

/**
 * Typed Registration Interface
 */
template<class T>
class TypedRegistrationInterface : public RegistrationInterface {
private:
	string name;

// HACK: should not be public, but inline access from StaticProvidingRegistrator
// needs
public:
	//HACK: remove it, here schouldn't be any object data
	T* data; //!< pointer to the data

	/**
	 * get the name
	 */
	const string getName() const {
		return name;
	}

	/**
	 * Constructor
	 * @param name_		name of this registration
	 */
	TypedRegistrationInterface(const string& name_) : name(name_), data(NULL) {}

	/**
	 * get data
	 */
	inline T& getData() const {
		//assert(data != NULL);
		return *data;
	}

	/**
	 * register at blackboard
	 */
	virtual Representation& registerAtBlackBoard() {
		DataHolder<T>& rep = get();
		data = &(*rep);
		return rep;
	}

	/**
	 * get data holder
	 */
	virtual DataHolder<T>& get() {
		return ModuleManager::GetInstance()->getRepresentation< DataHolder<T> >(name);
		//return ModuleManager::GetInstance()->template getRepresentation<DataHolder<T> >(getName());
	}

	/**
	 * get const reference of data holder
	 */
	virtual const DataHolder<T>& get() const {
		return ModuleManager::GetInstance()->getRepresentation< DataHolder<T> >(name);
		//return ModuleManager::GetInstance()->template getRepresentation<DataHolder<T> >(getName());
	}
};

#endif /* REGISTRATIONINTERFACE_H_ */
