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

class WorkerThread : public Thread {
public:
	WorkerThread(ThreadPool* manager, int threadID);
	virtual ~WorkerThread();

private:
	void execute(void* arg);

	ThreadPool* mManager;
	int mThreadID;
};

#endif /* WORKERTHREAD_H_ */
