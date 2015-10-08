#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

extern "C" {

class Thread {
  public:
    Thread();
    virtual ~Thread();

    int start(void *arg);
    void stop();

  protected:
    void Run(void *arg);
    static void* entryPoint(void *pthis);
    void *Arg() {return mArg;}
    void Arg(void *a) {mArg = a;}
    bool IsRunning();

  private:
    virtual bool setup();
    virtual void terminate();
    virtual void execute(void* arg) = 0;

    pthread_t mThreadId;
    pthread_mutex_t mRunningMutex;
    void *mArg;
    bool running;
};

}
#endif /*THREAD_H_*/
