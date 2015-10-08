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

class ThreadPool {
public:
	ThreadPool(ModuleManager* moduleManager);
	~ThreadPool();

	void DispatchWorkOrder(ModuleManager::ModuleData* workOrder);

	bool IsJobAvailable() const ;
	void WaitForJobPosted();
	void SignalJobTaken();
	void SignalJobDone(ModuleManager::ModuleData* module, bool wasExecuted);
	ModuleManager::ModuleData* GetWorkOrder();
	bool IsShuttingDown() const { return mIsShuttingDown; }
	void IncreaseLiveThreads() { mLiveThreads++; }
	void DecreaseLiveThreads() { mLiveThreads--; }
	void WaitForWorkOrderDone();
	int GetActiveJobs() const { return mActiveJobs; }

	pthread_mutex_t Mutex;
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
