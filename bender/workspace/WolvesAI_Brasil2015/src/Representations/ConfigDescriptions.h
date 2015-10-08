/*
 * ConfigDescriptions.h
 *
 *  Created on: 01.04.2014
 *      Author: Stefan Krupop
 */

#ifndef CONFIGDESCRIPTIONS_H_
#define CONFIGDESCRIPTIONS_H_

#include "../Blackboard/Printable.h"
#include "../Blackboard/Serializer.h"
#include <vector>
#include <string>

/**
 * Representation of descriptions for config entries
 */
class ConfigDescriptions: public Printable {
public:
	/**
	 * Constructor
	 */
	ConfigDescriptions();

	/**
	 * Destructor
	 */
	virtual ~ConfigDescriptions();

	/**
	 * add description to a config entry
	 * @param Section		name of section
	 * @param Setting		name of setting
	 * @param Description	description text
	 */
	void addDescription(string Section, string Setting, string Description);

	/**
	 * clear descriptions
	 */
	void clear();

	/**
	 * get the map of all descriptions with _Section_/_Setting_ as keys
	 * @return map of descriptions
	 */
	const map<string, string>* getDescriptions() const;

	/**
	 * print to a stream
	 */
	virtual void print(std::ostream& stream) const {
	}

private:
	//lint -e(1704)
	ConfigDescriptions(const ConfigDescriptions& cSource);				//!< Copy-Constructor
	ConfigDescriptions& operator=(const ConfigDescriptions& cSource);	//!< assignment

	map<string, string> mDescriptions; //!< the descriptions
};

/**
 * spezialization of Serializer for ConfigDescriptions
 */
template<>
class Serializer<ConfigDescriptions> {
public:
	/**
	 * serialize representation to stream
	 * @see Serializer::serialize
	 */
	static void serialize(const ConfigDescriptions& representation, std::ostream& stream);

	/**
	 * deserialize representation from stream
	 * @see Serializer::deserialize
	 */
	static void deserialize(std::istream& stream, ConfigDescriptions& representation);
};

#endif /* CONFIGDESCRIPTIONS_H_ */
