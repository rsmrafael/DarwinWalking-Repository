/*
 * ModuleManager.cpp
 *
 *  Created on: 14.08.2012
 *      Author: Stefan
 */

#include "ModuleManager.h"
#include "Module.h"
#include "ThreadPool.h"
#include "../Debugging/Debugger.h"
#include "../Config.h"
#include <google/protobuf/stubs/common.h>
#include <algorithm>
#include <sys/time.h>

using namespace std;

ModuleManager* ModuleManager::mInstance = 0;

ModuleManager* ModuleManager::GetInstance() {
	if (mInstance == 0) {
		mInstance = new ModuleManager();
	}
	return mInstance;
}

ModuleManager::ModuleManager() :
		mModuleExecutionMap(), mUnexecutedModules(), mAvailableRepresentations() {
	Debugger::INFO("ModuleManager", "Starting module manager");
	mBlackboard = new Blackboard();
	mThreadPool = new ThreadPool(this);
	mShutdownRequested = false;
	mPauseRequested = false;
	mPaused = false;
	mExecutionTimeMin = 32768;
	mExecutionTimeMax = 0;
	mCalculateExecutionTimeMetrics = false;
	mDependencyProblemCounter = 0;

	Config::getInstance()->registerConfigChangeHandler(this);
	this->configChanged();

	pthread_mutex_init(&mMutex, NULL);
	pthread_cond_init(&mCondPauseChanged, NULL);
	pthread_cond_init(&mCondPauseEnded, NULL);
}

ModuleManager::~ModuleManager() {
	pthread_cond_destroy(&mCondPauseEnded);
	pthread_cond_destroy(&mCondPauseChanged);
	pthread_mutex_destroy(&mMutex);

	delete mThreadPool;
	mUnexecutedModules.clear();
	mAvailableRepresentations.clear();
	Debugger::INFO("ModuleManager", "Shutting down modules...");
	for (Registry::iterator iter = mModuleExecutionMap.begin();
			iter != mModuleExecutionMap.end(); ++iter) {
		delete iter->module;
	}
	mModuleExecutionMap.clear();
	delete mBlackboard;
	Debugger::INFO("ModuleManager", "Execution times (min/max): %u ms / %u ms",
			mExecutionTimeMin, mExecutionTimeMax);
}

void ModuleManager::registerModule(Module* module) {
	registerModule(module, "", false);
}

void ModuleManager::registerModule(Module* module, string moduleGroup) {
	registerModule(module, moduleGroup, false);
}

void ModuleManager::registerModule(Module* module, string moduleGroup, bool noThreadPool) {
	pthread_mutex_lock(&mMutex);
	ModuleData modData;
	modData.module = module;
	modData.enabled = true;
	modData.executionTime = 0.0;
	modData.minExecutionTime = 9999.9;
	modData.maxExecutionTime = 0.0;
	modData.meanExecutionTime = 0.0;
	modData.executionCount = 0;
	modData.isExecuted = false;
	modData.noThreadPool = noThreadPool;
	modData.group = moduleGroup;
	mModuleExecutionMap.push_back(modData);
	mUnexecutedModules.push_back(&modData);
	Debugger::INFO("ModuleManager", "Added module %s",
			modData.module->getModuleName().c_str());
	pthread_mutex_unlock(&mMutex);
}

void ModuleManager::disableModule(const Module* module) {
	pthread_mutex_lock(&mMutex);
	for (Registry::iterator iter = mModuleExecutionMap.begin();
			iter != mModuleExecutionMap.end(); ++iter) {
		if (iter->module == module) {
			(*iter).enabled = false;
			mUnexecutedModules.remove(&(*iter));
			(*iter).isExecuted = false;
		}
	}
	Debugger::INFO("ModuleManager", "Disabled module %s",
			module->getModuleName().c_str());
	pthread_mutex_unlock(&mMutex);
}

void ModuleManager::enableModule(const Module* module) {
	pthread_mutex_lock(&mMutex);
	for (Registry::iterator iter = mModuleExecutionMap.begin();
			iter != mModuleExecutionMap.end(); ++iter) {
		if (iter->module == module) {
			(*iter).enabled = true;
			(*iter).isExecuted = false;
			mUnexecutedModules.push_back(&(*iter));
		}
	}
	Debugger::INFO("ModuleManager", "Enabled module %s",
			module->getModuleName().c_str());
	pthread_mutex_unlock(&mMutex);
}

bool ModuleManager::execute() {
//	Debugger::DEBUG("ModuleManager", "Executing modules...");

	timeval startTime;
	gettimeofday(&startTime, 0);

	pthread_mutex_lock(&mMutex);
	// Mark all modules as not executed
	// Clear provided representations, add those marked persistent again
	mAvailableRepresentations.clear();
	mUnexecutedModules.clear();
	for (Registry::iterator iter = mModuleExecutionMap.begin();
			iter != mModuleExecutionMap.end(); ++iter) {
		if (iter->enabled) {
			(*iter).isExecuted = false;
			mUnexecutedModules.push_back(&(*iter));

			list<Representation*> provided_persistent = (*iter).module->getProvidedPersistentRepresentations();
			for (list<Representation*>::iterator iter = provided_persistent.begin(); iter != provided_persistent.end(); ++iter) {
				mAvailableRepresentations.push_back((*iter)->getName());
			}

		}
	}
	pthread_mutex_unlock(&mMutex);

	do {
		// Post all jobs currently executable
		ModuleData* executableModule = getNextExecutableModule();
		if (executableModule == NULL && mThreadPool->GetActiveJobs() == 0) {

			pthread_mutex_lock(&mMutex);
			string unexed = "";
			for (list<ModuleData*>::iterator iter = mUnexecutedModules.begin();
					iter != mUnexecutedModules.end(); ++iter) {
				unexed += (*iter)->module->getModuleName();
				unexed += ", ";
			}
			if (unexed.size() > 1) {
				unexed.erase(unexed.size() - 2, unexed.size());
			}

			pthread_mutex_unlock(&mMutex);
			Debugger::ERR("ModuleManager", "Dependency problem number %d!",
					++mDependencyProblemCounter);
			Debugger::ERR("ModuleManager", "Some modules were not executed: %s",
					unexed.c_str());
			break;
		}
		do {
			if (executableModule != NULL) {
				if (executableModule->noThreadPool) {
				//	Debugger::DEBUG("ModuleManager", "Executing module %s", executableModule->module->getModuleName().c_str());
					timeval moduleStartTime;
					gettimeofday(&moduleStartTime, 0);
					bool wasExecuted = executableModule->module->execute();
					timeval moduleEndTime;
					gettimeofday(&moduleEndTime, 0);
					long seconds = moduleEndTime.tv_sec
							- moduleStartTime.tv_sec;
					long nseconds = moduleEndTime.tv_usec
							- moduleStartTime.tv_usec;
					executableModule->executionTime = seconds
							+ nseconds / 1000000.0;
					//Debugger::DEBUG("ModuleManager", "Executing module %s took %.3f s", executableModule->module->getModuleName().c_str(), executableModule->executionTime);
					ModuleExecuted(executableModule, wasExecuted);
				} else {
//					Debugger::DEBUG("ModuleManager", "Adding Job: %s", executableModule->module->getModuleName().c_str());
					executableModule->isExecuted = true;
					mThreadPool->DispatchWorkOrder(executableModule);
				}
			}
			executableModule = getNextExecutableModule();
		} while (executableModule != NULL);
//		Debugger::DEBUG("ModuleManager", "All currently executable jobs posted, waiting for job done");
		mThreadPool->WaitForWorkOrderDone();
//		Debugger::DEBUG("ModuleManager", "At least one job done");
	} while (!mUnexecutedModules.empty());
//	Debugger::DEBUG("ModuleManager", "Work done");

	timeval endTime;
	gettimeofday(&endTime, 0);
	long seconds = endTime.tv_sec - startTime.tv_sec;
	long nseconds = endTime.tv_usec - startTime.tv_usec;
	uint32_t exeTime = (uint32_t) (seconds * 1000)
			+ (uint32_t) (nseconds / 1000.0);
	//Debugger::DEBUG("ModuleManager", "Executing all modules took %u ms", exeTime);
	if (exeTime > mExecutionTimeMax) {
		mExecutionTimeMax = exeTime;
	}
	if (exeTime < mExecutionTimeMin) {
		mExecutionTimeMin = exeTime;
	}

	if (mPauseRequested) {
		mPaused = true;
		pthread_cond_signal(&mCondPauseChanged);
		pthread_mutex_lock(&mMutex);
		while (mPauseRequested) {
			pthread_cond_wait(&mCondPauseEnded, &mMutex);
		}
		pthread_mutex_unlock(&mMutex);
		mPaused = false;
		pthread_cond_signal(&mCondPauseChanged);
	}

	return !mShutdownRequested;
}

ModuleManager::ModuleData* ModuleManager::getNextExecutableModule() {
	pthread_mutex_lock(&mMutex);
	ModuleManager::ModuleData* ret = NULL;
	if (!mUnexecutedModules.empty()) {
		for (list<ModuleData*>::iterator iter = mUnexecutedModules.begin();
				iter != mUnexecutedModules.end(); ++iter) {
			if (!(*iter)->isExecuted) {
				bool dependenciesFulfilled = true;
				list<Representation*> required =
						(*iter)->module->getRequiredRepresentations();
				for (list<Representation*>::iterator it = required.begin();
						it != required.end(); ++it) {
					string repName = (*it)->getName();
					if (find(mAvailableRepresentations.begin(),
							mAvailableRepresentations.end(), repName)
							== mAvailableRepresentations.end()) {
						// Name of representation not found
						dependenciesFulfilled = false;
						break;
					}
				}
				if (dependenciesFulfilled) {
					if ((*iter)->group != "") {
						// Do not execute when module of same group is currently executed
						for (list<ModuleData*>::iterator iter2 = mUnexecutedModules.begin(); iter2 != mUnexecutedModules.end(); ++iter2) {
							if ((*iter2)->isExecuted) {
								if ((*iter2)->group == (*iter)->group) {
									dependenciesFulfilled = false;
								}
							}
						}
					}
				}
				if (dependenciesFulfilled) {
					ret = *iter;
					break;
				}
			}
		}
	}
	pthread_mutex_unlock(&mMutex);
	return ret;
}

void ModuleManager::ModuleExecuted(ModuleData* module, bool wasExecuted) {
	pthread_mutex_lock(&mMutex);
	// Mark as executed
	mUnexecutedModules.remove(module);
	// Provided representations are now available
	list<Representation*> provided =
			module->module->getProvidedRepresentations();
	for (list<Representation*>::iterator iter = provided.begin();
			iter != provided.end(); ++iter) {
		mAvailableRepresentations.push_back((*iter)->getName());
	}
	// Also remove all modules of same group if group is set
	if (wasExecuted && module->group != "") {
		if (!mUnexecutedModules.empty()) {
			list<ModuleData*>::iterator iter = mUnexecutedModules.begin();
			while (iter != mUnexecutedModules.end()) {
				if (!(*iter)->isExecuted) {
					if ((*iter)->group == module->group) {
						(*iter)->isExecuted = true;
						mUnexecutedModules.erase(iter++);
					} else {
						++iter;
					}
				} else {
					++iter;
				}
			}
		}
	}
	/*
	 string availableRepresentations = "";
	 for (list<Representation*>::iterator iter = mAvailableRepresentations.begin(); iter != mAvailableRepresentations.end(); ++iter) {
	 //availableRepresentations += (*iter)->getName() + "; ";
	 }
	 Debugger::DEBUG("ModuleManager", "Available representations: %s", availableRepresentations.c_str());
	 */

	if (mCalculateExecutionTimeMetrics) {
		if (module->executionTime > module->maxExecutionTime) {
			module->maxExecutionTime = module->executionTime;
		} else if (module->executionTime < module->minExecutionTime) {
			module->minExecutionTime = module->executionTime;
		}
		module->executionCount++;
		module->meanExecutionTime = module->meanExecutionTime - ((module->meanExecutionTime - module->executionTime) / module->executionCount);
		Debugger::addMetric("ExecutionTimesMin", module->module->getModuleName().c_str(), module->minExecutionTime * 1000.0);
		Debugger::addMetric("ExecutionTimesMean", module->module->getModuleName().c_str(), module->meanExecutionTime * 1000.0);
		Debugger::addMetric("ExecutionTimesMax", module->module->getModuleName().c_str(), module->maxExecutionTime * 1000.0);
	}

	pthread_mutex_unlock(&mMutex);
}

void ModuleManager::requestShutdown() {
	mShutdownRequested = true;
}

// If you think you need this function (and do not write something to do with debugging):
// PLEASE RE-THINK! This function should only be used for debugging purposes, never
// inside a module. So when your code accessing this function is under src/Modules/, please
// try to find another way to accomplish what you want. Accessing the Blackboard from an
// module without the ModuleManager knowing is BAD!
Blackboard* ModuleManager::getBlackboardDirect(bool iKnowIShouldNotDoThis) {
	if (iKnowIShouldNotDoThis) {
		return mBlackboard;
	}
	return NULL;
}

void ModuleManager::pauseExecutionAfterIteration() {
	mPauseRequested = true;
	pthread_mutex_lock(&mMutex);
	while (!mPaused) {
		pthread_cond_wait(&mCondPauseChanged, &mMutex);
	}
	pthread_mutex_unlock(&mMutex);
}

void ModuleManager::continueExecution() {
	mPauseRequested = false;
	pthread_cond_signal(&mCondPauseEnded);
	pthread_mutex_lock(&mMutex);
	while (mPaused) {
		pthread_cond_wait(&mCondPauseChanged, &mMutex);
	}
	pthread_mutex_unlock(&mMutex);
}

void ModuleManager::shutdown() {
	delete mInstance;
	mInstance = 0;

	google::protobuf::ShutdownProtobufLibrary();
}

void ModuleManager::setCreateCsv( bool create) {
	mCalculateExecutionTimeMetrics = create;
}

void ModuleManager::configChanged() {
	Debugger::INFO("ModuleManager", "config changed");
	ConfigFile *config = Config::getInstance();
#ifdef JENKINS
	mCalculateExecutionTimeMetrics = config->get<bool>("ModuleManager", "calculateExecutionTimeMetrics", true);
#else
	mCalculateExecutionTimeMetrics = config->get<bool>("ModuleManager", "calculateExecutionTimeMetrics", false);
#endif
}
