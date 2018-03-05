#ifndef TcpClientListener_H
#define TcpClientListener_H

#include "process/ThreadedProcess.h"

class TcpRegisteredClientInternal;

/**
 * 
 */

class TcpClientListener : virtual public ThreadedProcess
{
public:
    TcpClientListener(TcpRegisteredClientInternal* clt);
protected:
	virtual int threadStartFunction ();
	virtual int threadInnerFunction ();
	virtual int threadStopFunction ();
    TcpRegisteredClientInternal* mClient;
};

#endif
