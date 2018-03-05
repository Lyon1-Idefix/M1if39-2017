#include "internal/TcpClientInternal.h"

#include "internal/TcpServerListener.h"

#include "tcp/TcpClient.h"

#include <unistd.h>
#include <fcntl.h>
#include <cstring>


TcpClientInternal::TcpClientInternal(TcpClient* client,std::string host,int port,bool noThread) :
    mClient ( client ),
    mHost ( host ),
    mPort ( port ),
	mSocket(-1),
    mConnected ( false ),
	mNoThread ( noThread )
{
}

int TcpClientInternal::Connect ()
{
    DEBUGCALL;
	int m_sock;
	struct sockaddr_in m_addr;
	int status;
	int on;
	struct hostent *hostent_get_host;

    if ( mConnected ) return -1;
	hostent_get_host = gethostbyname(mHost.c_str ());

	m_sock = socket( PF_INET, SOCK_STREAM, 0);
	//fcntl(m_sock, F_SETFL, O_NONBLOCK);

	if( m_sock == -1 )
	{
        return -1;
	}
	
	// added by bmichoud
	on = 1;
	if( setsockopt( m_sock, IPPROTO_TCP, TCP_NODELAY, (char *) &on, sizeof(int)) < 0 )
		printf( "%s : no_delay impossible.\n", __FUNCTION__);
	// end added by bmichoud

	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(mPort);
	memcpy(&(m_addr.sin_addr.s_addr), hostent_get_host->h_addr, hostent_get_host->h_length);
	
	status = connect( m_sock, (struct sockaddr *) &m_addr, sizeof(m_addr));
	if( status != 0 ) 
	{
		close(m_sock);
	 	return -1;
	}
	if ( ! mNoThread )
	{
		mServerListenerTask = new TcpServerListener ( this );
		mServerListenerTask->start ();
	}
    mSocket = Socket ( m_sock );
    mConnected = true;
	mClient->OnConnected ();
    return TCP_NO_ERROR;
}

int TcpClientInternal::Disconnect ()
{
    DEBUGCALL;
    if ( ! mConnected ) return -1;
	if ( ! mNoThread )
	{
		mServerListenerTask->stop ();
		delete mServerListenerTask;
	}
    try { close ( mSocket.GetSocket () ); }
	catch (...) {}
    mConnected = false;
	mClient->OnDisconnected ();
    return TCP_NO_ERROR;
}

int TcpClientInternal::Send(const char* message, const int length)
{
    DEBUGCALL;
    if ( ! mConnected ) return -1;
    if ( mSocket.Send((char*)(&length),sizeof(int)) < 0 )
		disconnected ();
    if ( mSocket.Send(message,length) < 0 )
		disconnected ();
    return TCP_NO_ERROR;
}

int TcpClientInternal::Receive(char* message, int& length)
{
    DEBUGCALL;
    if ( mSocket.Receive((char*)(&length),sizeof(int)) == -1 ) {
        disconnected ();
        return -1;
    }
    if ( mSocket.Receive(message,length) == -1 ) {
        disconnected ();
        return -1;
    }
    return TCP_NO_ERROR;
}

void TcpClientInternal::OnReceive(char* message, int length)
{
	mClient->OnReceive (message,length);
}

void TcpClientInternal::disconnected () {
    DEBUGCALL;
    if ( ! mConnected ) return;
	if ( ! mNoThread )
	{
		mServerListenerTask->stop ();
		delete mServerListenerTask;
	}
    mConnected = false;
	mClient->OnDisconnected ();
}
