#include "tcp/TcpRegisteredClient.h"

#include "internal/TcpRegisteredClientInternal.h"
/**
 * 
 */

TcpRegisteredClient::TcpRegisteredClient(TcpServer* srv,Socket socket)
{
    mInternal = new TcpRegisteredClientInternal ( this, srv, socket );
}

TcpRegisteredClient::~TcpRegisteredClient()
{
    delete mInternal;
}

int TcpRegisteredClient::Send(const char* message, const int length)
{
    return mInternal->Send(message,length);
}

int TcpRegisteredClient::Receive(char* message, int& length)
{
    return mInternal->Receive(message,length);
}

bool TcpRegisteredClient::HasError ()
{
    return mInternal->HasError ();
}
