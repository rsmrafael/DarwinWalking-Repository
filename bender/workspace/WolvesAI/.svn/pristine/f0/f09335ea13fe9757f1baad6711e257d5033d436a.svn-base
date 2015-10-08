/*
 * Representation.h
 *
 *  Created on: 07.08.2012
 *      Author: Stefan Krupop
 */

#ifndef REPRESENTATION_H_
#define REPRESENTATION_H_

#include "Printable.h"
#include <string>
#include <list>

using namespace std;

class Module;

/**
 * a representation for the blackboard
 */
class Representation : public Printable {
public:
	/**
	 * Destructor
	 */
	virtual ~Representation() {};

	/**
	 * get the name of the representation
	 * @return name		name of the representation
	 */
	const string& getName() const { return mName; }

	/**
	 * register a providing module for this representation
	 * @param module	the module providing this representation
	 */
	void registerProvidingModule(const Module& module) {
		provided.push_back(&module);
	}

	/**
	 * unregister a providing module for this representation
	 * @param module	the module to be unregistered
	 */
	void unregisterProvidingModule(const Module& module) {
		provided.remove(&module);
	}

	/**
	 * register a persistent providing module
	 * @param module	the module providing this representation persistently
	 */
	void registerProvidingPersistentModule(const Module& module) {
		provided.push_back(&module);
		provided_persistent.push_back(&module);
	}

	/**
	 * unregister a persistent providing module
	 * @param module	the module to be unregistered from persistent modules
	 */
	void unregisterProvidingPersistentModule(const Module& module) {
		provided.remove(&module);
		provided_persistent.remove(&module);
	}

	/**
	 * register a requiring module
	 * @param module	the module requiring this representation
	 */
	void registerRequiringModule(const Module& module) {
		required.push_back(&module);
	}

	/**
	 * unregister a requiring module
	 * @param module	the module to be unregistered from requiring modules
	 */
	void unregisterRequiringModule(const Module& module) {
		required.remove(&module);
	}

	/**
	 * This method can be overwritten by a particular
	 * representation in order to stream out some specific (visible) data
	 * @param stream	the output stream
	 */
	virtual void print(ostream& stream) const {
		// use representation name as fallback
		stream << mName;
	}

	/**
	 * serialize representation to stream
	 * @param stream	output stream
	 */
	virtual void serialize(ostream& stream) const = 0;

	/**
	 * deserialize representation from stream
	 * @param stream	input stream
	 */
	virtual void deserialize(istream& stream) = 0;

protected:
	/**
	 * Constructor
	 * @param name	the name of the representation
	 */
	Representation(const string &name) : mName(name) {};

	// pointers to the providing and requiring modules
	list<const Module*> provided;				//!< the providing modules
	list<const Module*> provided_persistent;	//!< the persistent providing modules
	list<const Module*> required;				//!< the requiring modules

private:
	string mName;
};

#endif /* REPRESENTATION_H_ */
