#include "tcp/TcpClient.h"

#include "internal/TcpClientInternal.h"

/**
 * 
 */

TcpClient::TcpClient(std::string host,int port,bool noThread)
{
	mInternal = new TcpClientInternal ( this, host, port, noThread );
}

int TcpClient::Connect ()
{
    return mInternal->Connect ();
}

int TcpClient::Disconnect ()
{
    return mInternal->Disconnect ();
}

int TcpClient::Send(const char* message, const int length)
{
    return mInternal->Send (message,length);
}

int TcpClient::Receive(char* message, int& length)
{
    return mInternal->Receive(message,length);
}

bool TcpClient::Connected ()
{
    return mInternal->Connected ();
}
