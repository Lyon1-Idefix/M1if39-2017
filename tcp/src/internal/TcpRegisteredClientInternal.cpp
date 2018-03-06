#include "internal/TcpRegisteredClientInternal.h"

#include "tcp/TcpServer.h"
#include "internal/TcpClientListener.h"

/**
 * 
 */

TcpRegisteredClientInternal::TcpRegisteredClientInternal(TcpRegisteredClient* client,TcpServer* srv,Socket socket,bool noThread) :
    mError(false),
    mSocket ( socket ),
    mClient ( client ),
    mServer ( srv ),
    mNoThread ( noThread )
{
    DEBUGCALL;
}

TcpRegisteredClientInternal::~TcpRegisteredClientInternal()
{
    DEBUGCALL;
}

int TcpRegisteredClientInternal::Send(const char* message, const int length)
{
    DEBUGCALL;
    mSocket.Send((char*)(&length),sizeof(int));
    mSocket.Send(message,length);
    return TCP_NO_ERROR;
}

int TcpRegisteredClientInternal::Receive(char* message, int& length)
{
    DEBUGCALL;
    if ( mSocket.Receive((char*)(&length),sizeof(int)) == -1 ) {
        Disconnected ();
        return -1;
    }
    if ( mSocket.Receive(message,length) == -1 ) {
        Disconnected ();
        return -1;
    }
    return TCP_NO_ERROR;
}

int TcpRegisteredClientInternal::Start()
{
    DEBUGCALL;
	if ( ! mNoThread )
	{
        mListenerTask = new TcpClientListener(this);
        mListenerTask->start ();
    }
    return TCP_NO_ERROR;
}

int TcpRegisteredClientInternal::Stop()
{
    DEBUGCALL;
	if ( ! mNoThread )
	{
        mListenerTask->stop ();
        delete mListenerTask;
    }
    close(mSocket.GetSocket());
    return TCP_NO_ERROR;
}

void TcpRegisteredClientInternal::OnReceive(char* message, int length)
{
    mServer->OnReceive(mClient,message,length);
}
