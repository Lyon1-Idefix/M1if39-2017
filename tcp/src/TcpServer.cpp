#include "tcp/TcpServer.h"

#include "internal/TcpServerInternal.h"

/**
 * 
 */

TcpServer::TcpServer(int port, bool noClientThread)
{
    mInternal = new TcpServerInternal ( this, port, noClientThread );
}

int TcpServer::Reserve()
{
    return mInternal->Reserve ();
}

int TcpServer::Release ()
{
    return mInternal->Release ();
}

int TcpServer::Start ()
{
    return mInternal->Start ();
}

int TcpServer::Stop ()
{
    return mInternal->Stop ();
}

int TcpServer::Update ()
{
    return mInternal->Update ();
}

int TcpServer::Send(const char* message, const int length)
{
    return mInternal->Send (message,length);
}

bool TcpServer::Ready ()
{
    return mInternal->Ready ();
}

bool TcpServer::Running ()
{
    return mInternal->Running ();
}
