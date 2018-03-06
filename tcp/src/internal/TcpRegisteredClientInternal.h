#ifndef TcpRegisteredClientInternal_H
#define TcpRegisteredClientInternal_H

#include "tcp/Tcp.h"

class TcpServer;
class TcpRegisteredClient;
class TcpClientListener;

class TcpRegisteredClientInternal
{
public:
    ~TcpRegisteredClientInternal ();
    int Send(const char* message, const int length);
    int Receive(char* message, int& length);
    TcpRegisteredClientInternal(TcpRegisteredClient* client,TcpServer* srv,Socket socket,bool noThread = false);
    int Start ();
    int Stop ();
    inline Socket ConnectedSocket () { return mSocket; }
    inline bool HasError () { return mError; }
    inline void Disconnected () { mError = true; }
    void OnReceive(char* message, int length);
    bool mError;
	Socket mSocket;
    TcpRegisteredClient* mClient;
    TcpServer* mServer;
    TcpClientListener* mListenerTask;
    bool mNoThread;
};

#endif
