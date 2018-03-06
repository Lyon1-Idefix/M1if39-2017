#ifndef TcpServerInternal_H
#define TcpServerInternal_H

#include "tcp/Tcp.h"

class TcpServer;
class TcpClientListener;
class TcpConnectionListener;
class TcpRegisteredClient;

class TcpServerInternal
{
public:
	TcpServerInternal(TcpServer* server, int port,bool noClientThread = false);
	int Reserve ();
	int Release ();
    int Start ();
    int Stop ();
    int Update ();
	int Send(const char* message, const int length);
    
    inline Socket ConnectedSocket () { return mSocket; }
    inline int Port () { return mPort; }
    inline bool Ready () { return mReady; }
    inline bool Running () { return mRunning; }

    int OnConnect(TcpRegisteredClient* client);
    void OnDisconnect(TcpRegisteredClient* client);
    void OnReceive(TcpRegisteredClient* client,char* message, int length);
    int addClient(int sock);
    TcpServer* mServer;
	int mPort;
    Socket mSocket;
	bool mReady;
	bool mRunning;
    TcpConnectionListener* mConnectionTask;
    std::vector<TcpRegisteredClient*> mRegisteredClients;
    bool mNoClientThread;
    friend class TcpConnectionListener;
};

#endif
