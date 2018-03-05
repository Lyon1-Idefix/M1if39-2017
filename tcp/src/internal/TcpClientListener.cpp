#include "internal/TcpClientListener.h"

#include "internal/TcpRegisteredClientInternal.h"

TcpClientListener::TcpClientListener(TcpRegisteredClientInternal* clt) :
    mClient ( clt )
{
    DEBUGCALL;
}

int TcpClientListener::threadStartFunction ()
{
    DEBUGCALL;
    return TCP_NO_ERROR;
}

int TcpClientListener::threadInnerFunction ()
{
    DEBUGCALL;
    int length;
    char* buffer;
    if ( mClient->ConnectedSocket ().HasIncomingData () )
    {
        if ( mClient->ConnectedSocket ().Receive((char*)(&length),sizeof(int)) <=0 ) {
            mClient->Disconnected ();
            return -1;
        }
        buffer = new char[length];
        if ( mClient->ConnectedSocket ().Receive(buffer,length) <= 0 ) {
            mClient->Disconnected ();
            return -1;
        }
        mClient->OnReceive ( buffer, length );
    }
    return TCP_NO_ERROR;
}

int TcpClientListener::threadStopFunction ()
{
    DEBUGCALL;
    return TCP_NO_ERROR;
}
