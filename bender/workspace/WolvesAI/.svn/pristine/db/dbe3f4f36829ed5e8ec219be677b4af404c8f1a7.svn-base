/*
 * DataHolder.h
 *
 *  Created on: 08.08.2012
 *      Author: Stefan Krupop
 */

#ifndef DATAHOLDER_H_
#define DATAHOLDER_H_

#include "Serializer.h"
#include "Printable.h"

template<class T>
class DataHolder : public Representation {
public:
	DataHolder(const std::string& name) : Representation(name), mData() {}
	DataHolder() : Representation(typeid(T).name()) {}
	virtual ~DataHolder() {}

	T& operator*() {
		//lint -e(1536)
		return mData;
	}

	const T& operator*() const {
		return mData;
	}

	/**
	 * wrap the print, fromDataStream and toDataStream of the data member
	 */
	virtual void print(std::ostream& stream) const {
		const T* asPointer = &mData;
		const Printable* asPrintable = dynamic_cast<const Printable*>(asPointer);
		if (asPrintable != NULL) {
			stream << *asPrintable;
		} else {
			// use representation by default
			stream << getName();
		}
	} //end print

	void serialize(std::ostream& stream) const {
		Serializer<T>::serialize(mData, stream);
	}

	void deserialize(std::istream& stream) {
		Serializer<T>::deserialize(stream, mData);
	}

private:
	// creates an object of the data
	// (this requires the class T to have a default constructor)
	T mData;

	DataHolder<T>& operator=(const DataHolder<T>& cSource);
};

#endif /* DATAHOLDER_H_ */
