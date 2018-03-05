#include "process/ThreadedProcess.h"

ThreadedProcess::ThreadedProcess () :
	mRunning ( false )
{
}
ThreadedProcess::~ThreadedProcess ()
{
}
int ThreadedProcess::start ()
{
	if ( mRunning ) return TASK_ERROR_ALREADY_RUNNING;
	if( pthread_create( &mThreadID, NULL, ThreadedProcess::threadLoop, (void*) this) != 0 )
	{
		return TASK_ERROR_THREAD_CREATE;
	}
	mRunning = true;
	return TASK_NO_ERROR;
}

int ThreadedProcess::stop ()
{
	if ( ! mRunning ) return TASK_ERROR_NOT_RUNNING;
	mRunning = false;
	pthread_join(mThreadID, NULL);
	return TASK_NO_ERROR;
}

void* ThreadedProcess::threadLoop(void* ptr)
{
	ThreadedProcess *pThis = (ThreadedProcess *) ptr;
	pThis->threadStartFunction ();
	while ( pThis->mRunning )
	{
		if ( pThis->threadInnerFunction () != 0 )
			break;
	}
	pThis->threadStopFunction ();
	return NULL;
}
