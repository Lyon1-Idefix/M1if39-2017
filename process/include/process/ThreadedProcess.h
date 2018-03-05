#ifndef ThreadedTask_H
#define ThreadedTask_H

#include <iostream>
#include <pthread.h>

#define TASK_NO_ERROR								0
#define TASK_ERROR_ALREADY_RUNNING					0
#define TASK_ERROR_THREAD_CREATE					0
#define TASK_ERROR_NOT_RUNNING						0

#ifndef DEBUGCALL
#define DEBUGCALL //std::clog << __FUNCTION__ << "[" << __FILE__ << ":" << __LINE__ << "]" << std::endl
#endif

class ThreadedProcess
{
public:
	ThreadedProcess ();
	virtual ~ThreadedProcess();
	int start ();
	int stop ();
protected:
	virtual int threadStartFunction () = 0;
	virtual int threadInnerFunction () = 0;
	virtual int threadStopFunction () = 0;
private:
	static void* threadLoop(void* ptr);
	bool mRunning;
	pthread_t mThreadID;
};

#endif
