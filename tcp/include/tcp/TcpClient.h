#ifndef TcpClient_H
#define TcpClient_H

#include "tcp/Tcp.h"

class TcpClientInternal;

class TcpClient
{
public:
	TcpClient(std::string host,int port,bool noThread = false);

	int Connect ();
	int Disconnect ();

	int Send(const char* message, const int length);
    int Receive(char* message, int& length);

    bool Connected ();

    virtual void OnConnected() = 0;
    virtual void OnDisconnected() = 0;
    virtual void OnReceive(char* message, int length) = 0;
private:
	TcpClientInternal* mInternal;
};

#endif
