#include <iostream>
#include <map>

using namespace std;

#include "tcp/TcpServer.h"
#include "tcp/TcpRegisteredClient.h"

class ChatServer : public TcpServer
{
public:
	ChatServer(int port) : TcpServer(port) {}
	int OnConnect(TcpRegisteredClient* client)
	{
		char name[255];
		int lg;
		client->Receive(name,lg);
		std::string message = name;
		message += " is connected";
		Send ( message.c_str (), message.length ()+1 );
		names[client]=name;
		return 0;
	}
	void OnDisconnect(TcpRegisteredClient* client)
	{
		std::string nmessage = names[client];
		nmessage += " is disconnected";
		Send ( nmessage.c_str (), nmessage.length ()+1 );
		names.erase ( client );
	}
	void OnReceive(TcpRegisteredClient* client,char* message, int length)
	{
		std::string nmessage = names[client];
		nmessage += " said ";
		nmessage += message;
		Send ( nmessage.c_str (), nmessage.length ()+1 );
	}
	std::map < TcpRegisteredClient*, std::string > names;
};

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define STDIN 0  // file descriptor for standard input

bool HasInput ()
{
    struct timeval tv;
    fd_set readfds;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    FD_ZERO(&readfds);
    FD_SET(STDIN, &readfds);
    select(STDIN+1, &readfds, NULL, NULL, &tv);
    if (FD_ISSET(STDIN, &readfds)) return true;
    return false;
}

int main ( int argc, char** argv )
{
	bool done = false;
	ChatServer server (1234);
	server.Reserve ();
	server.Start ();
	while ( ! done ) {
		if ( HasInput () ) {
			std::string command;
			cin >> command;
			if ( command == "quit" ) done = true;
		}
		server.Update ();
	}
	server.Stop ();
	server.Release ();
}
