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

class ModuleManager : public ConfigChangeListener {
public:
	struct ModuleData {
		Module* module;
		bool enabled;
		double executionTime;
		double minExecutionTime;
		double maxExecutionTime;
		double meanExecutionTime;
		uint32_t executionCount;
		bool isExecuted;
		bool noThreadPool;
		std::string group;
	};

    static ModuleManager* GetInstance();
	void shutdown(void) const;

	template<class T>
	T& getRepresentation(const std::string& name) throw(RepresentationTypeMismatch) {
		return mBlackboard->getRepresentation<T>(name);
	}

	bool execute();
	void requestShutdown();

	void registerModule(Module* module);
	void registerModule(Module* module, std::string moduleGroup);
	void registerModule(Module* module, std::string moduleGroup, bool noThreadPool);
	void disableModule(const Module* module);
	void enableModule(const Module* module);

	Blackboard* getBlackboardDirect(bool iKnowIShouldNotDoThis);
	void pauseExecutionAfterIteration();
	void continueExecution();

	void ModuleExecuted(ModuleData*, bool wasExecuted);

	void setCreateCsv( bool create);

	virtual void configChanged();

protected:
	ModuleManager();
	~ModuleManager();

private:
	ModuleData* getNextExecutableModule();

	//lint -e(1704)
    ModuleManager(const ModuleManager& cSource);
    ModuleManager& operator=(const ModuleManager& cSource);

    static ModuleManager* mInstance;

    Blackboard* mBlackboard;
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
};

#endif /* MODULEMANAGER_H_ */
