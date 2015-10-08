/*
 * Serializer.h
 *
 *  Created on: 08.08.2012
 *      Author: Stefan Krupop
 */

#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include <iostream>
#include <typeinfo>

#include "../Debugging/Debugger.h"
#include "Representation.h"

/**
 * Serializer is the base class for all serializer implementations
 */
template<class T>
class Serializer {
public:
	static void serialize(const T& representation, std::ostream& stream) {
		//lint --e{1764}
		Debugger::ERR("Serializer", "No serializer for %s", typeid(T).name());
		// do nothing
	}

	static void deserialize(std::istream& stream, T& representation) {
		//lint --e{1764}
		Debugger::ERR("Serializer", "No deserializer for %s", typeid(T).name());
		// do nothing
	}
};

template<>
class Serializer<Representation> {
public:
	static void serialize(const Representation& representation, std::ostream& stream) {
		representation.serialize(stream);
	}

	static void deserialize(std::istream& stream, Representation& representation) {
		representation.deserialize(stream);
	}
};

#endif /* SERIALIZER_H_ */
