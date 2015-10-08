/*
 * WorkerThread.cpp
 *
 *  Created on: 03.09.2012
 *      Author: Stefan Krupop
 */

#include "WorkerThread.h"
#include "ModuleManager.h"
#include "Module.h"
#include <sys/time.h>

WorkerThread::WorkerThread(ThreadPool* manager, int threadID) : mManager(manager), mThreadID(threadID) {
}

WorkerThread::~WorkerThread() {
	mManager = NULL;
}

void WorkerThread::execute(void* arg) {
	pthread_cleanup_push((dispatch_func_t)pthread_mutex_unlock, (void *)&(mManager->Mutex));

//		Debugger::DEBUG("WorkerThread", "%d: Locking mutex", mThreadID);
		pthread_mutex_lock(&(mManager->Mutex));
//		Debugger::DEBUG("WorkerThread", "%d: Got mutex", mThreadID);

		mManager->IncreaseLiveThreads();

		while (IsRunning()) {
			// Wait for job
//			Debugger::DEBUG("WorkerThread", "%d: Waiting for job", mThreadID);
			while (!mManager->IsJobAvailable() && !mManager->IsShuttingDown()) {
//        		Debugger::DEBUG("WorkerThread", "%d: WaitForJobPosted", mThreadID);
				mManager->WaitForJobPosted();
//        		Debugger::DEBUG("WorkerThread", "%d: Wait for job posted done", mThreadID);
			}

//        	Debugger::DEBUG("WorkerThread", "%d: Got job", mThreadID);
			// Check if we should just end instead of working
			if (mManager->IsShuttingDown()) {
				break;
			}

			// while we find work to do
			ModuleManager::ModuleData* executableModule = mManager->GetWorkOrder();
			mManager->SignalJobTaken();

			// Yield mutex so other jobs can be posted
			pthread_mutex_unlock(&(mManager->Mutex));
//			Debugger::DEBUG("WorkerThread", "%d: Unlocked mutex", mThreadID);

			// Run the job we've taken
			//Debugger::DEBUG("WorkerThread", "%d: Executing module %s", mThreadID, executableModule->module->getModuleName().c_str());
			timeval startTime;
			gettimeofday(&startTime, 0);
			bool wasExecuted = executableModule->module->execute();
			timeval endTime;
			gettimeofday(&endTime, 0);
			long seconds = endTime.tv_sec - startTime.tv_sec;
			long nseconds = endTime.tv_usec - startTime.tv_usec;
			executableModule->executionTime = seconds + (nseconds / 1000000.0);
			//Debugger::DEBUG("WorkerThread", "%d: Executing module %s took %.3f s", mThreadID, executableModule->module->getModuleName().c_str(), executableModule->executionTime);

			// Grab mutex so we can grab posted job, or (if no job is posted)
			// begin waiting for next posting.
//			Debugger::DEBUG("WorkerThread", "%d: Locking Mutex 2", mThreadID);
			pthread_mutex_lock(&(mManager->Mutex));
//			Debugger::DEBUG("WorkerThread", "%d: Got Mutex 2", mThreadID);
			mManager->SignalJobDone(executableModule, wasExecuted);
//			Debugger::DEBUG("WorkerThread", "%d: Signaled job done", mThreadID);
		};
		mManager->DecreaseLiveThreads();

		// We're not really taking a job ... but this signals the destroyer
		//   that one thread has exited, so it can keep on destroying.
		mManager->SignalJobTaken();
		pthread_mutex_unlock(&(mManager->Mutex));

		pthread_cleanup_pop(0);
}
