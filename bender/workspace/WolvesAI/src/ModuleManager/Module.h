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
#ifdef ROBOTAUDIO
#include "../Debugging/RobotAudio.h"
#endif
#include <string>
#include <list>

using namespace std;

/**
 * Module is an interface for modules working with representations of the blackboard
 */
class Module {
public:
	virtual ~Module() { }

	/**
	 * the execute function is called every cycle,
	 * when all the required representations were provided
	 */
	virtual bool execute() = 0;

	/**
	 * get the name of the module
	 */
	const string& getModuleName() const {
		return (*this).mModuleName;
	}

	/**
	 * get a list of all required representations for this module
	 */
	const list<Representation*>& getRequiredRepresentations() const {
		return mRequired;
	}

	/**
	 * get a list of all provided representations for this module
	 */
	const list<Representation*>& getProvidedRepresentations() const {
		return mProvided;
	}

	/**
	 * get a list of all persistent provided representations for this module
	 */
	const list<Representation*>& getProvidedPersistentRepresentations() const {
		return mProvidedPersistent;
	}

protected:
	/**
	 * Constructor
	 * @param name		name of the module
	 */
	Module(string name) : mModuleName(name) {
	}

	/**
	 * Default-Constructor
	 */
	Module() : mModuleName("invalid module") {
		// should never be here
	}

	/**
	 * register a providing RepresentationMap
	 * @param lst		provided RepresentationMap
	 */
	void registerProviding(const RepresentationMap& lst) {
		RepresentationMap::const_iterator iter = lst.begin();
		for (; iter != lst.end(); ++iter) {
			// init the actual dependency to the black board
			Representation& representation = (*iter).second->registerAtBlackBoard();
			mProvided.push_back(&representation);
			representation.registerProvidingModule(*this);
		}
	}

	/**
	 * register a persistent providing RepresentationMap
	 * @param lst		persistent provided RepresentationMap
	 */
	void registerProvidingPersistent(const RepresentationMap& lst) {
		RepresentationMap::const_iterator iter = lst.begin();
		for (; iter != lst.end(); ++iter) {
			// init the actual dependency to the black board
			Representation& representation = (*iter).second->registerAtBlackBoard();
			mProvidedPersistent.push_back(&representation);
			representation.registerProvidingPersistentModule(*this);
		}
	}

	/**
	 * register a requiring RepresentationMap
	 * @param lst		required RepresentationMap
	 */
	void registerRequiring(const RepresentationMap& lst) {
		RepresentationMap::const_iterator iter = lst.begin();
		for (; iter != lst.end(); ++iter) {
			// init the actual dependency to the black board
			Representation& representation = (*iter).second->registerAtBlackBoard();
			mRequired.push_back(&representation);
			representation.registerRequiringModule(*this);
		}
	}

	/**
	 * unregister a providing RepresentationMap
	 * @param lst		providing RepresentationMap
	 */
	void unregisterProviding(const RepresentationMap& lst) {
		RepresentationMap::const_iterator iter = lst.begin();
		for (; iter != lst.end(); ++iter) {
			// init the actual dependency to the black board
			Representation& representation = (*iter).second->registerAtBlackBoard();
			mProvided.remove(&representation);
			representation.unregisterProvidingModule(*this);
		}
	}

	/**
	 * unregister a persistent providing RepresentationMap
	 * @param lst		providing RepresentationMap
	 */
	void unregisterProvidingPersistent(const RepresentationMap& lst) {
		RepresentationMap::const_iterator iter = lst.begin();
		for (; iter != lst.end(); ++iter) {
			// init the actual dependency to the black board
			Representation& representation = (*iter).second->registerAtBlackBoard();
			mProvidedPersistent.remove(&representation);
			representation.unregisterProvidingPersistentModule(*this);
		}
	}

	/**
	 * unregister a requiring RepresentationMap
	 * @param lst		requiring RepresentationMap
	 */
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
	string mModuleName;							//!< name of this module
	// pointers to the provided and required representations
	list<Representation*> mProvided;			//!< provided representations
	list<Representation*> mProvidedPersistent;	//!< persistent provided representations
	list<Representation*> mRequired;			//!< required representations
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
