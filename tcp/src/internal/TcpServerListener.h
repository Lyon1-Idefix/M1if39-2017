#ifndef TcpServerListener_H
#define TcpServerListener_H

#include "process/ThreadedProcess.h"

class TcpClientInternal;

/**
 * 
 */


class TcpServerListener : virtual public ThreadedProcess
{
public:
    TcpServerListener(TcpClientInternal* clt);
protected:
	virtual int threadStartFunction ();
	virtual int threadInnerFunction ();
	virtual int threadStopFunction ();
    TcpClientInternal* mClient;

};
#endif
