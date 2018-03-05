#ifndef TcpClientInternal_H
#define TcpClientInternal_H

#include "tcp/Tcp.h"

class TcpClient;
class TcpServerListener;

class TcpClientInternal
{
public:
	TcpClientInternal(TcpClient* client,std::string host,int port,bool noThread = false);

	int Connect ();
	int Disconnect ();
	
	int Send(const char* message, const int length);
    int Receive(char* message, int& length);
    void OnReceive(char* message, int length);
    
	inline std::string Host () { return mHost; }
    inline int Port () { return mPort; }
    inline Socket ConnectedSocket () { return mSocket; }
    inline bool Connected () { return mConnected; }
protected:
	void disconnected ();
private:
    TcpClient* mClient;
    std::string mHost;
	int mPort;
	Socket mSocket;
	bool mConnected;
    TcpServerListener* mServerListenerTask;
	bool mNoThread;
	friend class TcpServerListener;
};

#endif
