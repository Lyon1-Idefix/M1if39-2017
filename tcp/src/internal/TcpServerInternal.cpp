#include "internal/TcpServerInternal.h"

#include "tcp/TcpServer.h"
#include "tcp/TcpRegisteredClient.h"

#include "internal/TcpConnectionListener.h"
#include "internal/TcpRegisteredClientInternal.h"

/**
 * 
 */

TcpServerInternal::TcpServerInternal(TcpServer* server, int port,bool noClientThread) :
    mServer(server),
	mPort ( port ),
    mSocket ( -1 ),
    mReady(false),
    mRunning(false),
    mNoClientThread(noClientThread)
{
}

int TcpServerInternal::Reserve()
{
    DEBUGCALL;
    int on;
    struct sockaddr_in m_addr;
    int bind_return;
    int mSocketID;

    if (mReady)
    {
        return TCP_OPEN_ERROR_READY;
    }
    mSocketID = socket( PF_INET, SOCK_STREAM, 0);

    if( mSocketID == -1 )
    {
        return TCP_OPEN_ERROR_SOCKET_RESERVED;
    }

    on = 1;
    if( setsockopt( mSocketID, SOL_SOCKET, SO_REUSEADDR, (const char*) &on, sizeof(on)) < 0 )
    {
        return TCP_OPEN_ERROR_SOCKET_CONFIGURATION;
    }
    if( setsockopt( mSocketID, IPPROTO_TCP, TCP_NODELAY, (char *) &on, sizeof(int)) < 0 )
    {
        return TCP_OPEN_ERROR_SOCKET_NODELAY;
    }
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = INADDR_ANY;
    m_addr.sin_port = htons ( mPort );
    bind_return = bind( mSocketID, (struct sockaddr *) &m_addr, sizeof(m_addr));
    if( bind_return == -1 ) 
    {
        return TCP_OPEN_ERROR_SOCKET_BIND;
    }
    mSocket = Socket ( mSocketID );
    mReady = true;
    return TCP_NO_ERROR;
}

int TcpServerInternal::Release ()
{
    DEBUGCALL;
    for ( unsigned int i = 0 ; i < mRegisteredClients.size () ; i ++ )
    {
        mRegisteredClients[i]->Internal()->Stop ();
        delete mRegisteredClients[i];
    }
    mRegisteredClients.clear ();
    close(mSocket.GetSocket ());
    return TCP_NO_ERROR;
}

int TcpServerInternal::Start ()
{
    DEBUGCALL;
    if ( ! mReady ) return TCP_RUN_ERROR_NOT_READY;
    if ( mRunning ) return TCP_ERROR_ALREADY_RUNNING;
    mConnectionTask = new TcpConnectionListener(this);
    mConnectionTask->start ();
    mRunning = true;
    return TCP_NO_ERROR;
}

int TcpServerInternal::Stop ()
{
    DEBUGCALL;
    if ( ! mRunning ) return TCP_ERROR_NOT_RUNNING;
    mConnectionTask->stop ();
    delete mConnectionTask;
    mRunning = false;
    return TCP_NO_ERROR;
}

int TcpServerInternal::Update ()
{
    std::vector < TcpRegisteredClient* > toRemove;
    for ( unsigned int i = 0 ; i < mRegisteredClients.size () ; i ++ )
    {
        if ( mRegisteredClients[i]->Internal()->HasError() )
            toRemove.push_back ( mRegisteredClients[i] );
    }
    for ( unsigned int i = 0 ; i < toRemove.size () ; i ++ )
    {
        mRegisteredClients.erase (std::find (mRegisteredClients.begin (), mRegisteredClients.end (), toRemove[i]));
    }
    for ( unsigned int i = 0 ; i < toRemove.size () ; i ++ )
    {
        OnDisconnect ( toRemove[i] );
        toRemove[i]->Internal()->Stop ();
        delete toRemove[i];
    }
    return TCP_NO_ERROR;
}

/*
int TcpServerInternal::removeClient (TcpRegisteredClient* client)
{
    DEBUGCALL;
    if ( ! mRunning ) return TCP_ERROR_NOT_RUNNING;
    mConnectionTask->stop ();
    delete mConnectionTask;
    mRunning = false;
    return TCP_NO_ERROR;
}
*/

    int TcpServerInternal::OnConnect(TcpRegisteredClient* client) { return mServer->OnConnect(client); }
    void TcpServerInternal::OnDisconnect(TcpRegisteredClient* client) { mServer->OnDisconnect(client); }
    void TcpServerInternal::OnReceive(TcpRegisteredClient* client,char* message, int length) { mServer->OnReceive(client,message,length); }

int TcpServerInternal::addClient(int sock)
{
    DEBUGCALL;
    TcpRegisteredClient* client = new TcpRegisteredClient(mServer,sock,mNoClientThread);
    if ( OnConnect ( client ) == 0 )
    {
        mRegisteredClients.push_back(client);
        client->Internal()->Start ();
    }
    else delete client;
    return TCP_NO_ERROR;
}

int TcpServerInternal::Send(const char* message, const int length)
{
    DEBUGCALL;
    for ( unsigned int i = 0 ; i < mRegisteredClients.size () ; i ++ )
    {
        if ( mRegisteredClients[i]->Internal()->Send(message,length) != TCP_NO_ERROR )
            mRegisteredClients[i]->Internal()->Disconnected ();
    }
    return TCP_NO_ERROR;
}
