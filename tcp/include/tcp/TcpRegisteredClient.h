#ifndef TcpRegisteredClient_H
#define TcpRegisteredClient_H

#include "tcp/Tcp.h"

class TcpServer;
class TcpRegisteredClientInternal;

class TcpRegisteredClient
{
public:
    ~TcpRegisteredClient ();
    
    int Send(const char* message, const int length);
    int Receive(char* message, int& length);

    bool HasError ();
protected:
    inline TcpRegisteredClientInternal* Internal() { return mInternal; }
    TcpRegisteredClient(TcpServer* srv,Socket socket,bool noThread = false);
private:
    TcpRegisteredClientInternal* mInternal;
    friend class TcpServer;
    friend class TcpServerInternal;
};

#endif
