/*
 * ThreadPool.cpp
 *
 *  Created on: 13.09.2012
 *      Author: Stefan Krupop
 */

#include "ThreadPool.h"
#include "WorkerThread.h"
#include "ModuleManager.h"
#include "../Debugging/Debugger.h"
#include "../Config.h"

#define MIN_THREADS			2
#define MAX_WORK_ORDERS		20

ThreadPool::ThreadPool(ModuleManager* moduleManager)
: mModuleManager(moduleManager),
  mWorkers(),
  mWorkOrders()
{
	int threadsPerCPU = Config::GetInt("Main", "threadsPerCPU", 2);//4
	long numCPU;
#ifdef WIN32
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	numCPU = (long)sysinfo.dwNumberOfProcessors;
#else
	numCPU = sysconf( _SC_NPROCESSORS_ONLN );
#endif
	int numberOfThreads = (int)numCPU * threadsPerCPU;
	if (numberOfThreads < MIN_THREADS) {
		Debugger::WARN("ModuleManager",
				"Number of threads too low: %d (numCPU: %d), changed to minimum!",
				numberOfThreads, numCPU);
		numberOfThreads = MIN_THREADS;
	}
	Debugger::INFO("ModuleManager", "Found %i CPUs, using %i threads", numCPU, numberOfThreads);
	mIsShuttingDown = false;
    pthread_mutex_init(&Mutex, NULL);
    pthread_cond_init(&mCondJobPosted, NULL);
    pthread_cond_init(&mCondJobTaken, NULL);
    pthread_cond_init(&mCondJobDone, NULL);
    mActiveJobs = 0;
    mLiveThreads = 0;
	for (int i = 0; i < numberOfThreads; ++i) {
		WorkerThread* t = new WorkerThread(this, i + 1);
		mWorkers.push_back(t);
		t->start(t);
	}
}

ThreadPool::~ThreadPool() {
	Debugger::INFO("ThreadPool", "Shutting down worker threads...");
    pthread_mutex_lock(&Mutex);
	mIsShuttingDown = true;
	while (mLiveThreads > 0) {
		pthread_cond_signal(&mCondJobPosted);
		pthread_cond_wait(&mCondJobTaken, &Mutex);
	}
	pthread_mutex_unlock(&Mutex);

	for (list<WorkerThread*>::iterator iter = mWorkers.begin(); iter != mWorkers.end(); ++iter) {
		(*iter)->stop();
		delete *iter;
	}
    pthread_cond_destroy(&mCondJobPosted);
    pthread_cond_destroy(&mCondJobTaken);
    pthread_cond_destroy(&mCondJobDone);
    pthread_mutex_destroy(&Mutex);
    mModuleManager = NULL;
}

void ThreadPool::DispatchWorkOrder(ModuleManager::ModuleData* workOrder) {
	pthread_cleanup_push((dispatch_func_t)pthread_mutex_unlock, (void *) &Mutex);

	// Grab the mutex
//		Debugger::DEBUG("ThreadPool", "DispatchWorkOrder: Locking mutex");
		pthread_mutex_lock(&Mutex);
//		Debugger::DEBUG("ThreadPool", "DispatchWorkOrder: Got mutex");

		while (mWorkOrders.size() > MAX_WORK_ORDERS) {
			Debugger::DEBUG("ThreadPool", "DispatchWorkOrder: Waiting for free space in queue");
			pthread_cond_signal(&mCondJobPosted);
			pthread_cond_wait(&mCondJobTaken, &Mutex);
		}

	// Finally, there's room to post a job. Do so and signal workers.
		mWorkOrders.push_back(workOrder);

//		Debugger::DEBUG("ThreadPool", "DispatchWorkOrder: Signal job posted");
		pthread_cond_signal(&mCondJobPosted);

	// Yield mutex so a worker can pick up the job
		pthread_mutex_unlock(&Mutex);
//		Debugger::DEBUG("ThreadPool", "DispatchWorkOrder: Unlocked Mutex");

		pthread_cleanup_pop(0);
}

void ThreadPool::WaitForWorkOrderDone() {
//	Debugger::DEBUG("ThreadPool", "WaitForWorkOrderDone: Locking Mutex");
	pthread_mutex_lock(&Mutex);
//	Debugger::DEBUG("ThreadPool", "WaitForWorkOrderDone: Got Mutex");
	if (mActiveJobs > 0 || !mWorkOrders.empty()) {
		pthread_cond_wait(&mCondJobDone, &Mutex);
	}
	pthread_mutex_unlock(&Mutex);
//	Debugger::DEBUG("ThreadPool", "WaitForWorkOrderDone: Unlocked Mutex");
}

bool ThreadPool::IsJobAvailable() const {
	return !mWorkOrders.empty();
}

void ThreadPool::WaitForJobPosted() {
	pthread_cond_wait(&mCondJobPosted, &Mutex);
}

void ThreadPool::SignalJobTaken() {
	mActiveJobs++;
	pthread_cond_signal(&mCondJobTaken);
}

void ThreadPool::SignalJobDone(ModuleManager::ModuleData* module, bool wasExecuted) {
	mModuleManager->ModuleExecuted(module, wasExecuted);
	mActiveJobs--;
	pthread_cond_signal(&mCondJobDone);
}

ModuleManager::ModuleData* ThreadPool::GetWorkOrder() {
	ModuleManager::ModuleData* ret = mWorkOrders.back();
	mWorkOrders.pop_back();
	return ret;
}
