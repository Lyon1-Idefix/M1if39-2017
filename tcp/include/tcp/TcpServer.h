#ifndef TcpServer_H
#define TcpServer_H

#include "tcp/Tcp.h"

class TcpServer;
class TcpClientListener;
class TcpConnectionListener;
class TcpRegisteredClient;
class TcpServerInternal;

class TcpServer
{
public:
	TcpServer(int port, bool noClientThread = false);
    
	int Reserve ();
	int Release ();
    int Start ();
    int Stop ();
    int Update ();

	int Send(const char* message, const int length);
    
    bool Ready ();
    bool Running ();

    virtual int OnConnect(TcpRegisteredClient* client) = 0;
    virtual void OnDisconnect(TcpRegisteredClient* client) = 0;
    virtual void OnReceive(TcpRegisteredClient* client,char* message, int length) = 0;
private:
    TcpServerInternal* mInternal;
};

#endif
