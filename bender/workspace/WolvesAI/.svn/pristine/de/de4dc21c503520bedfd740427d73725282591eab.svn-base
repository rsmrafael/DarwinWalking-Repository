/*
 * Module.h
 *
 *  Created on: 21.08.2012
 *      Author: Stefan Krupop
 */

#ifndef MODULE_H_
#define MODULE_H_

#include "StaticRegistry.h"
#include "../Blackboard/Blackboard.h"
#include "../Blackboard/Representation.h"
#include "../Debugging/Debugger.h"
#include "../Debugging/DebugDrawer.h"
#include <string>
#include <list>

using namespace std;

class Module {
public:
	virtual ~Module() { }

	virtual bool execute() = 0;

	const string& getModuleName() const {
		return (*this).mModuleName;
	}

	const list<Representation*>& getRequiredRepresentations() const {
		return mRequired;
	}

	const list<Representation*>& getProvidedRepresentations() const {
		return mProvided;
	}

	const list<Representation*>& getProvidedPersistentRepresentations() const {
		return mProvidedPersistent;
	}

protected:
	Module(string name) : mModuleName(name) {
	}

	Module() : mModuleName("invalid module") {
		// should never be here
	}

	 void registerProviding(const RepresentationMap& lst) {
		RepresentationMap::const_iterator iter = lst.begin();
		for (; iter != lst.end(); ++iter) {
			// init the actual dependency to the black board
			Representation& representation = (*iter).second->registerAtBlackBoard();
			mProvided.push_back(&representation);
			representation.registerProvidingModule(*this);
		}
	}

	void registerProvidingPersistent(const RepresentationMap& lst) {
		RepresentationMap::const_iterator iter = lst.begin();
		for (; iter != lst.end(); ++iter) {
			// init the actual dependency to the black board
			Representation& representation = (*iter).second->registerAtBlackBoard();
			mProvidedPersistent.push_back(&representation);
			representation.registerProvidingPersistentModule(*this);
		}
	}

	void registerRequiring(const RepresentationMap& lst) {
		RepresentationMap::const_iterator iter = lst.begin();
		for (; iter != lst.end(); ++iter) {
			// init the actual dependency to the black board
			Representation& representation = (*iter).second->registerAtBlackBoard();
			mRequired.push_back(&representation);
			representation.registerRequiringModule(*this);
		}
	}

	void unregisterProviding(const RepresentationMap& lst) {
		RepresentationMap::const_iterator iter = lst.begin();
		for (; iter != lst.end(); ++iter) {
			// init the actual dependency to the black board
			Representation& representation = (*iter).second->registerAtBlackBoard();
			mProvided.remove(&representation);
			representation.unregisterProvidingModule(*this);
		}
	}

	void unregisterProvidingPersistent(const RepresentationMap& lst) {
		RepresentationMap::const_iterator iter = lst.begin();
		for (; iter != lst.end(); ++iter) {
			// init the actual dependency to the black board
			Representation& representation = (*iter).second->registerAtBlackBoard();
			mProvidedPersistent.remove(&representation);
			representation.unregisterProvidingPersistentModule(*this);
		}
	}

	void unregisterRequiring(const RepresentationMap& lst) {
		RepresentationMap::const_iterator iter = lst.begin();
		for (; iter != lst.end(); ++iter) {
			// init the actual dependency to the black board
			Representation& representation = (*iter).second->registerAtBlackBoard();

			mRequired.remove(&representation);
			representation.unregisterRequiringModule(*this);
		}
	}

private:
	string mModuleName;
	// pointers to the provided and required representations
	list<Representation*> mProvided;
	list<Representation*> mProvidedPersistent;
	list<Representation*> mRequired;
};

#define BEGIN_DECLARE_MODULE(moduleName) \
  class moduleName##Base: public Module, private StaticRegistry<moduleName##Base> {

#define REQUIRE(typeName, representationName) \
  private: \
  class representationName##StaticRequiringRegistrator : public StaticRequiringRegistrator<typeName> { \
  public: representationName##StaticRequiringRegistrator() : StaticRequiringRegistrator<typeName>(#representationName){} \
  } the##representationName; \
  protected: \
  inline const typeName& get##representationName() const { \
    static const typeName& representation = the##representationName.get(); \
    return representation; \
  }

#define PROVIDE(typeName, representationName) \
  private: \
  class representationName##StaticProvidingRegistrator : public StaticProvidingRegistrator<typeName> { \
  public: representationName##StaticProvidingRegistrator() : StaticProvidingRegistrator<typeName>(#representationName){} \
  } the##representationName; \
  protected: \
  inline typeName& get##representationName() const { \
    return the##representationName.getData(); \
  }

#define PROVIDE_PERSISTENT(typeName, representationName) \
  private: \
  class representationName##StaticProvidingPersistentRegistrator : public StaticProvidingPersistentRegistrator<typeName> { \
  public: representationName##StaticProvidingPersistentRegistrator() : StaticProvidingPersistentRegistrator<typeName>(#representationName){} \
  } the##representationName; \
  protected: \
  inline typeName& get##representationName() const { \
    return the##representationName.getData(); \
  }

#define END_DECLARE_MODULE(moduleName) \
  public: \
    moduleName##Base(): Module(#moduleName) { \
      registerRequiring(static_requiring_registry); \
      registerProviding(static_providing_registry); \
      registerProvidingPersistent(static_providing_persistent_registry); \
    } \
    virtual ~moduleName##Base() { \
      unregisterRequiring(static_requiring_registry); \
      unregisterProviding(static_providing_registry); \
      unregisterProvidingPersistent(static_providing_persistent_registry); \
    } \
    static string const className() { return #moduleName; } \
  };

#endif /* MODULE_H_ */
