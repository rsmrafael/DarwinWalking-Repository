/*
 * Blackboard.h
 *
 *  Created on: 07.08.2012
 *      Author: Stefan Krupop
 */

#ifndef BLACKBOARD_H_
#define BLACKBOARD_H_

#include "BlackboardData.h"
#include "BlackboardDataHolder.h"
#include "BlackboardExceptions.h"
#include <map>
#include <vector>
#include <string>

/**
 * the blackboard handling all representations
 */
class Blackboard {
public:
	/**
	 * Constructor
	 */
	Blackboard() {};

	/**
	 * Destructor
	 */
	~Blackboard();

	/**
	 * clean up blackboard
	 */
	void shutdown();

	/**
	 * serialize blackboard to an output stream
	 * @param stream 	the output stream
	 * @param TOConly	only serialize names (true) or complete representations (false)
	 */
	void serialize(std::ostream& stream, bool TOConly = false);

	/**
	 * serialize blackboard to an output stream
	 * @param stream 	the output stream
	 * @param TOConly	only serialize names (true) or complete representations (false)
	 * @param items		the items to be serialized to stream
	 */
	void serialize(std::ostream& stream, bool TOConly, std::vector<std::string>& items);

	/**
	 * deserialize representation with given name to an input stream
	 * @param name		the name of representation
	 * @param stream 	the input stream
	 */
	void deserialize(std::string name, std::istream& stream);

	/**
	 *  returns a reference to a representation stored
	 *  on the blackboard. Assertion is thrown if no
	 *  according representation is available.
	 */
	template<class T>
	const T& getRepresentation(const std::string& name) const throw(RepresentationTypeMismatch, RepresentationNotExisting) {
		// search for the representation
		Registry::const_iterator iter = mRegistry.find(name);

		// create Representation, if necessary
		if (iter == mRegistry.end()) {
			throw RepresentationNotExisting("Representation not existing: " + name + " of type " + typeid(T).name() + "is not registered and cannot be created.");
		}

		// retrieve the representation and try to cast
		BlackboardData* data = iter->second;
		BlackboardDataHolder<T>* typedData = dynamic_cast<BlackboardDataHolder<T>*>(data);

		// couldn't cast the type
		if (typedData == NULL) {
			throw RepresentationTypeMismatch("Representation type mismatch: " + name + " is registered as " + data->getTypeName() + ", but " + typeid(T).name() + " is requested.");
		}

		return **typedData;
	}

	/**
	 *  returns a reference to a representation stored
	 *  on the blackboard. A new instance is created if the required
	 *  representation is not existing.
	 */
	template<class T>
	T& getRepresentation(const std::string& name) throw(RepresentationTypeMismatch) {
		// search for the representation
		Registry::iterator iter = mRegistry.find(name);

		// create Representation, if necessary
		if (iter == mRegistry.end()) {
			BlackboardDataHolder<T>* typedData = new BlackboardDataHolder<T>(name);
			mRegistry[name] = typedData;
			return **typedData;
		}

		// retrieve the representation and try to cast
		BlackboardData* data = iter->second;
		BlackboardDataHolder<T>* typedData = dynamic_cast<BlackboardDataHolder<T>*>(data);

		// couldn't cast the type
		if (typedData == NULL) {
			throw RepresentationTypeMismatch("Representation type mismatch: " + name + " is registered as " + data->getTypeName() + ", but " + typeid(T).name() + " is requested.");
		}

		return **typedData;
	}

	/**
	 * get a reference to a representation by its name
	 * @return representation with given name
	 */
	template<class T>
	T& operator[](const std::string& name) { return getRepresentation<T>(name); }

	/**
	 * get a constant reference to a representation by its name
	 * @return representation with given name
	 */
	template<class T>
	const T& operator[](const std::string& name) const { return getRepresentation<T>(name); }

private:
	typedef std::map<std::string, BlackboardData*> Registry;

	Registry mRegistry;	//!< the data of the blackboard (representations sorted by name)
};

#endif /* BLACKBOARD_H_ */
