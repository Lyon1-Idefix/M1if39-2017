#ifndef Tcp_H
#define Tcp_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>	// for sockaddr_in, INADDR_ANY
#include <arpa/inet.h>	// for inet_pton(...)
#include <unistd.h>		// for close(...)
#include <netinet/tcp.h>
#include <netdb.h>

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <vector>
#include <algorithm>

#define TCP_NO_ERROR							  0
#define TCP_OPEN_ERROR_SOCKET_RESERVED			 -1
#define TCP_OPEN_ERROR_SOCKET_CONFIGURATION		 -2
#define TCP_OPEN_ERROR_SOCKET_NODELAY			 -3
#define TCP_OPEN_ERROR_SOCKET_BIND				 -4
#define TCP_OPEN_ERROR_READY					 -5
#define TCP_RUN_ERROR_NOT_READY					-10
#define TCP_RUN_ERROR_LISTEN					-11
#define TCP_ERROR_ALREADY_RUNNING                 -12
#define TCP_ERROR_NOT_RUNNING                 -13

#define DEBUGCALL //std::clog << __FUNCTION__ << "[" << __FILE__ << ":" << __LINE__ << "]" << std::endl

class Socket
{
public:
    Socket(int sock) : mSocketID(sock) {}
    int Send(const char* message, int length) {
        int total_size = 0;
        int current_size;
        do 
        {
            current_size = send( mSocketID, &(message[total_size]), length-total_size, 0);//MSG_NOSIGNAL);
            if ( current_size == 0 ) break;
            if( current_size <= 0 ) 
                return -1;

            total_size += current_size;
        } 
        while( total_size < length );
        return total_size;
    }
    int Receive(char* message, int length) {
        int total_size = 0;
        int current_size;
        do 
        {
            current_size = recv( mSocketID, &(message[total_size]), length-total_size, 0);
            if ( current_size == 0 ) break;
            if( current_size <= 0 )
                return -1;

            total_size += current_size;
        } 
        while( total_size < length );
        return total_size;
    }
    inline int GetSocket () { return mSocketID; }
    bool HasIncomingData () {
        struct timeval tv;
        fd_set readfds;
        tv.tv_sec = 0;
        tv.tv_usec = 100000;
        FD_ZERO(&readfds);
        FD_SET(mSocketID, &readfds);
        select(mSocketID+1, &readfds, NULL, NULL, &tv);
        if (FD_ISSET(mSocketID, &readfds)) return true;
        return false;
    }
protected:
    int mSocketID;
};

#endif
