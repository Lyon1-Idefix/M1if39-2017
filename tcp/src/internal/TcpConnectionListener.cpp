#include "internal/TcpConnectionListener.h"

#include "internal/TcpServerInternal.h"

TcpConnectionListener::TcpConnectionListener(TcpServerInternal* srv) :
    mServer ( srv )
{
    DEBUGCALL;
}

int TcpConnectionListener::threadStartFunction ()
{
    DEBUGCALL;
    int listen_return;
    if (!mServer->Ready()) return -1;
    listen_return = listen( mServer->ConnectedSocket().GetSocket (), 5);
    if( listen_return == -1 ) 
    {
        return TCP_RUN_ERROR_LISTEN;
    }
    return TCP_NO_ERROR;
}

int TcpConnectionListener::threadInnerFunction ()
{
    DEBUGCALL;
    struct sockaddr_in m_addr;
    int addr_length;
    addr_length = sizeof(m_addr);
    if ( mServer->ConnectedSocket().HasIncomingData () )
    {
        int nv_sock = accept( mServer->ConnectedSocket().GetSocket (), (struct sockaddr *) &m_addr, (socklen_t *) &addr_length);
        mServer->addClient(nv_sock);
    }
    return TCP_NO_ERROR;
}

int TcpConnectionListener::threadStopFunction ()
{
    DEBUGCALL;
    return TCP_NO_ERROR;
}
