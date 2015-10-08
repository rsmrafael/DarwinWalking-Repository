/*
 * ModuleManager.h
 *
 *  Created on: 14.08.2012
 *      Author: Stefan
 */

#ifndef MODULEMANAGER_H_
#define MODULEMANAGER_H_

#include "../Blackboard/Blackboard.h"
#include "../Blackboard/Representation.h"
#include "../ConfigChangeListener.h"
#include <stdint.h>
#include <string>
#include <list>
#include <pthread.h>

class ThreadPool;

/**
 * the ModuleManager handles all modules
 */
class ModuleManager : public ConfigChangeListener {
public:

	/**
	 * data of a module
	 */
	struct ModuleData {
		Module* module;				//!< pointer to the module
		bool enabled;				//!< module is enabled
		double executionTime;		//!< last execution time
		double minExecutionTime;	//!< minimal execution time
		double maxExecutionTime;	//!< maximal execution time
		double meanExecutionTime;	//!< mean execution time
		uint32_t executionCount;	//!< counter for execution calls
		bool isExecuted;			//!< is module executed
		bool noThreadPool;			//!< module uses thread pool (false) or not (true)
		std::string group;			//!< group name
	};

	/**
	 * get singleton instance
	 */
    static ModuleManager* GetInstance();

    /**
     * clean up everything
     */
	static void shutdown();

	/**
	 * get representation
	 * @param name 		name of representation
	 * @return representation
	 */
	template<class T>
	T& getRepresentation(const std::string& name) throw(RepresentationTypeMismatch) {
		return mBlackboard->getRepresentation<T>(name);
	}

	/**
	 * execute one cycle, so execute all registered modules
	 * @return false if shutdown request
	 */
	bool execute();

	/**
	 * request a shutdown
	 */
	void requestShutdown();

	/**
	 * register a new module
	 * @param module	the module to be added
	 */
	void registerModule(Module* module);

	/**
	 * register a new module of a module group
	 * Only one module of each module group is executed
	 * @param module		the module  to be added
	 * @param moduleGroup	the name of the module group
	 */
	void registerModule(Module* module, std::string moduleGroup);

	/**
	 * register a new module of a module group
	 * Only one module of each module group is executed
	 * @param module		the module  to be added
	 * @param moduleGroup	the name of the module group
	 * @param noThreadPool	if true, no use of thread pool for execution
	 */
	void registerModule(Module* module, std::string moduleGroup, bool noThreadPool);

	/**
	 * disable a module
	 * @param module	module to disable
	 */
	void disableModule(const Module* module);

	/**
	 * enable a module
	 * @param module	module to enable
	 */
	void enableModule(const Module* module);

	/**
	 * Get direct access to the blackboard.
	 * Bad coding style, please avoid to use this!
	 * @param iKnowIShouldNotDoThis 	only set true, if you are sure you want this
	 */
	Blackboard* getBlackboardDirect(bool iKnowIShouldNotDoThis);

	/**
	 * pause the execution of modules
	 */
	void pauseExecutionAfterIteration();

	/**
	 * continue the execution of modules
	 */
	void continueExecution();

	/**
	 * set status of execution
	 * @param module		the module to change execution status
	 * @param wasExecuted	if true, module was executed successfully
	 */
	void ModuleExecuted(ModuleData* module, bool wasExecuted);

	/**
	 * enable or disable creating metrics
	 * @param create	if true, metrics are created
	 */
	void setCreateCsv( bool create);

	/**
	 * @see ConfigChangeListener::configChanged()
	 */
	virtual void configChanged();

protected:
	ModuleManager();
	~ModuleManager();

private:
	ModuleData* getNextExecutableModule();

	//lint -e(1704)
    ModuleManager(const ModuleManager& cSource);
    ModuleManager& operator=(const ModuleManager& cSource);

    static ModuleManager* mInstance;	//!< singleton instance

    Blackboard* mBlackboard;		//!< access to blackboard
    bool mShutdownRequested;
    bool mPauseRequested;
    bool mPaused;
    uint32_t mExecutionTimeMin;
    uint32_t mExecutionTimeMax;
    bool mCalculateExecutionTimeMetrics;

    typedef std::list<ModuleData> Registry;
    Registry mModuleExecutionMap;
    std::list<ModuleData*> mUnexecutedModules;
    std::list<std::string> mAvailableRepresentations;

    ThreadPool* mThreadPool;

	pthread_mutex_t mMutex;
	//lint --e{18} symbol redeclared ???
    pthread_cond_t mCondPauseChanged;
    pthread_cond_t mCondPauseEnded;

    int mDependencyProblemCounter;
};

#endif /* MODULEMANAGER_H_ */
