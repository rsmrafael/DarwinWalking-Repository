/*
 * ThreadPool.h
 *
 *  Created on: 13.09.2012
 *      Author: Stefan Krupop
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "ModuleManager.h"
#include <list>
#include <pthread.h>

using namespace std;

typedef void (* dispatch_func_t)(void *);

class WorkerThread;

/**
 * ThreadPool provides a pool of threads for the execution of modules
 */
class ThreadPool {
public:
	/**
	 * Constructor
	 * @param moduleManager		pointer to the moduleManager
	 */
	ThreadPool(ModuleManager* moduleManager);
	~ThreadPool();

	/**
	 * dispatch the work order for a module
	 * @param workOrder		the data of the module to dispatch
	 */
	void DispatchWorkOrder(ModuleManager::ModuleData* workOrder);

	/**
	 * check if there is a job available
	 * @return true if a job is available
	 */
	bool IsJobAvailable() const;

	/**
	 * wait for a job posted
	 */
	void WaitForJobPosted();

	/**
	 * signal job is taken
	 */
	void SignalJobTaken();

	/**
	 * signal job is done
	 * @param module		data of the finished module
	 * @param wasExecuted	true if module was executed successfully
	 */
	void SignalJobDone(ModuleManager::ModuleData* module, bool wasExecuted);

	/**
	 * get the work order
	 * @return module data for next executable module
	 */
	ModuleManager::ModuleData* GetWorkOrder();

	/**
	 * check if is shutting down
	 * @return true if shutting down
	 */
	bool IsShuttingDown() const { return mIsShuttingDown; }

	/**
	 * increase the number of live threads
	 */
	void IncreaseLiveThreads() { mLiveThreads++; }

	/**
	 * decrease the number of live threads
	 */
	void DecreaseLiveThreads() { mLiveThreads--; }

	/**
	 * wait for work order done
	 */
	void WaitForWorkOrderDone();

	/**
	 * get number of active jobs
	 * @return number of active jobs
	 */
	int GetActiveJobs() const { return mActiveJobs; }

	pthread_mutex_t Mutex;	//!< mutex for thread pool
private:
	//lint -e(1704)
    ThreadPool(const ThreadPool& cSource);
    ThreadPool& operator=(const ThreadPool& cSource);

    ModuleManager* mModuleManager;
    bool mIsShuttingDown;
    int mLiveThreads;
    list<WorkerThread*> mWorkers;
    pthread_cond_t mCondJobPosted;
    pthread_cond_t mCondJobTaken;
    pthread_cond_t mCondJobDone;
    list<ModuleManager::ModuleData*> mWorkOrders;
    int mActiveJobs;
};

#endif /* THREADPOOL_H_ */
