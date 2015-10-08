/*
 * StaticRegistry.h
 *
 *  Created on: 27.08.2012
 *      Author: Stefan Krupop
 */

#ifndef STATICREGISTRY_H_
#define STATICREGISTRY_H_

#include "RegistrationInterface.h"
#include <map>

using namespace std;

template<class T>
//lint --e{1790}
class StaticRegistry {
public:
	virtual ~StaticRegistry() {
		for (RepresentationMap::iterator iter = static_providing_registry.begin(); iter != static_providing_registry.end(); ++iter) {
			delete iter->second;
		}
		static_providing_registry.clear();
		for (RepresentationMap::iterator iter = static_providing_persistent_registry.begin(); iter != static_providing_persistent_registry.end(); ++iter) {
			delete iter->second;
		}
		static_providing_persistent_registry.clear();
		for (RepresentationMap::iterator iter = static_requiring_registry.begin(); iter != static_requiring_registry.end(); ++iter) {
			delete iter->second;
		}
		static_requiring_registry.clear();
	}
protected:
	static RepresentationMap static_providing_registry;
	static RepresentationMap static_providing_persistent_registry;
	static RepresentationMap static_requiring_registry;

	template<class TYPE_WHAT>
	class StaticRequiringRegistrator {
	private:
		//lint -esym(768,StaticRegistry<*>::StaticRequiringRegistrator<*>::data)
		TypedRegistrationInterface<TYPE_WHAT>* data;

	public:
		StaticRequiringRegistrator() : data(NULL) {
		}

		StaticRequiringRegistrator(const string& name) : data(NULL) {
			if (static_requiring_registry.find(name) == static_requiring_registry.end()) {
				static_requiring_registry[name] = new TypedRegistrationInterface<TYPE_WHAT>(name);
			}
			// HACK
			data = (TypedRegistrationInterface<TYPE_WHAT>*)static_requiring_registry[name];
		}

		const TYPE_WHAT& get() const {
			return *(data->get());
		}
	};

	template<class TYPE_WHAT>
	class StaticProvidingRegistrator {
	private:
		//lint -esym(768,StaticRegistry<*>::StaticProvidingRegistrator<*>::data)
		TypedRegistrationInterface<TYPE_WHAT>* data;

	public:
		StaticProvidingRegistrator() : data(NULL) {}

		StaticProvidingRegistrator(const string& name) : data(NULL) {
			if (static_providing_registry.find(name) == static_providing_registry.end()) {
				static_providing_registry[name] = new TypedRegistrationInterface<TYPE_WHAT>(name);
			}
			// HACK
			data = (TypedRegistrationInterface<TYPE_WHAT>*) static_providing_registry[name];
		}

		inline TYPE_WHAT& get() const {
			return *(data->get());
		}

		inline TYPE_WHAT& getData() const {
			//assert(data->data != NULL);
			return *(data->data);
		}
	};

	template<class TYPE_WHAT>
	class StaticProvidingPersistentRegistrator {
	private:
		//lint -esym(768,StaticRegistry<*>::StaticProvidingPersistentRegistrator<*>::data)
		TypedRegistrationInterface<TYPE_WHAT>* data;

	public:
		StaticProvidingPersistentRegistrator() : data(NULL) {}

		StaticProvidingPersistentRegistrator(const string& name) : data(NULL) {
			if (static_providing_persistent_registry.find(name) == static_providing_persistent_registry.end()) {
				static_providing_persistent_registry[name] = new TypedRegistrationInterface<TYPE_WHAT>(name);
				static_providing_registry[name] = new TypedRegistrationInterface<TYPE_WHAT>(name);
			}
			// HACK
			data = (TypedRegistrationInterface<TYPE_WHAT>*) static_providing_registry[name];
		}

		inline TYPE_WHAT& get() const {
			return *(data->get());
		}

		inline TYPE_WHAT& getData() const {
			//assert(data->data != NULL);
			return *(data->data);
		}
	};
};

template<class T>
RepresentationMap StaticRegistry<T>::static_providing_registry = RepresentationMap();
template<class T>
RepresentationMap StaticRegistry<T>::static_providing_persistent_registry = RepresentationMap();
template<class T>
RepresentationMap StaticRegistry<T>::static_requiring_registry = RepresentationMap();

#endif /* STATICREGISTRY_H_ */
