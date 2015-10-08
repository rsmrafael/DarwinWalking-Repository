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

class ConfigDescriptions: public Printable {
public:
	ConfigDescriptions();
	virtual ~ConfigDescriptions();

	void addDescription(string Section, string Setting, string Description);
	void clear();
	const map<string, string>* getDescriptions() const;

	virtual void print(std::ostream& stream) const {
	}

private:
	//lint -e(1704)
	ConfigDescriptions(const ConfigDescriptions& cSource);
	ConfigDescriptions& operator=(const ConfigDescriptions& cSource);

	map<string, string> mDescriptions;
};

template<>
	class Serializer<ConfigDescriptions> {
	public:
		static void serialize(const ConfigDescriptions& representation, std::ostream& stream);
		static void deserialize(std::istream& stream, ConfigDescriptions& representation);
	};

#endif /* CONFIGDESCRIPTIONS_H_ */
