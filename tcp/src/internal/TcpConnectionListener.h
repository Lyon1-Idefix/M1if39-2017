#ifndef TcpConnectionListener_H
#define TcpConnectionListener_H

#include "process/ThreadedProcess.h"

class TcpServerInternal;

/**
 * 
 */

class TcpConnectionListener : virtual public ThreadedProcess
{
public:
    TcpConnectionListener(TcpServerInternal* srv);
protected:
	virtual int threadStartFunction ();
	virtual int threadInnerFunction ();
	virtual int threadStopFunction ();
    TcpServerInternal* mServer;
};

#endif
