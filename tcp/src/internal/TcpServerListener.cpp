#include "internal/TcpServerListener.h"

#include "internal/TcpClientInternal.h"

TcpServerListener::TcpServerListener(TcpClientInternal* clt) :
    mClient ( clt )
{
    DEBUGCALL;
}

int TcpServerListener::threadStartFunction ()
{
    DEBUGCALL;
    return TCP_NO_ERROR;
}

int TcpServerListener::threadInnerFunction ()
{
    DEBUGCALL;
    int length;
    char* buffer;
    if ( mClient->ConnectedSocket ().HasIncomingData () )
    {
        if ( mClient->ConnectedSocket ().Receive((char*)(&length),sizeof(int)) <=0 ) {
            mClient->disconnected ();
            return -1;
        }
        buffer = new char[length];
        if ( mClient->ConnectedSocket ().Receive(buffer,length) <=0 ) {
            mClient->disconnected ();
            return -1;
        }
        mClient->OnReceive ( buffer, length );
    }
    return TCP_NO_ERROR;
}

int TcpServerListener::threadStopFunction ()
{
    DEBUGCALL;
    return TCP_NO_ERROR;
}
