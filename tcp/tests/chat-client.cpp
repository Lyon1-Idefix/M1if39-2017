#include <iostream>
#include <cstring>

using namespace std;

#include "tcp/TcpClient.h"

class MyClient : public TcpClient
{
public:
	MyClient(std::string nm,std::string host,int port) : TcpClient(host,port), name ( nm ) {}
    void OnConnected()
    {
        std::cout << "you are now connected as " << name << std::endl;
    }
    void OnDisconnected()
    {
        std::cout << "you have been disconnected" << std::endl;
    }
	void OnReceive(char* message, int length)
	{
		std::cout << message << std::endl;
	}
    std::string name;
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
    char name[255];
    std::string message;
	bool done = false;
    cout << "name: "; cin >> name;
	MyClient client (name,"127.0.0.1",1234);
	client.Connect ();
    client.Send(name,strlen(name));
	while ( client.Connected () ) {
        if (HasInput())
        {
            cin >> message;
            if ( message == "quit" ) break;
            client.Send(message.c_str (),message.length ()+1);
        }
	}
	client.Disconnect ();
}
