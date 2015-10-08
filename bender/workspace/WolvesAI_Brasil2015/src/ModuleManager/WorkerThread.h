/*
 * WorkerThread.h
 *
 *  Created on: 03.09.2012
 *      Author: Stefan Krupop
 */

#ifndef WORKERTHREAD_H_
#define WORKERTHREAD_H_

#include "../Thread.h"
#include "ThreadPool.h"

/**
 * A thread for use in ThreadPool to execute modules
 */
class WorkerThread : public Thread {
public:

	/**
	 * Constructor
	 * @param manager		thread pool for managing the execution of the threads
	 * @param threadID		id of this WorkerThread
	 */
	WorkerThread(ThreadPool* manager, int threadID);
	virtual ~WorkerThread();

private:
	void execute(void* arg);

	ThreadPool* mManager;
	int mThreadID;
};

#endif /* WORKERTHREAD_H_ */
