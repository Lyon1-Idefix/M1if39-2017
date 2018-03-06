#include <iostream>

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#include "tcp/TcpClient.h"

typedef struct {
    int cols;
    int rows;
    int type;
    int depth;
    int nChannels;
    int step;
} VideoSize;

Mat mCurrentFrame;
VideoSize mSize;
bool dataReceived = false;
bool processingData = false;

class VideoClient : public TcpClient
{
public:
	VideoClient(std::string host,int port) : TcpClient(host,port) {}
    void OnConnected()
    {
    }
    void OnDisconnected()
    {
    }
	void OnReceive(char* message, int length)
	{
        while ( processingData ) pthread_yield ();
        memcpy ( (char*) mCurrentFrame.ptr (), message, length );
        dataReceived = true;
	}
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
    int lg;
    int ok = 1;
    bool done = false;

    int id = atoi ( argv[1] );
	VideoClient client ("127.0.0.1",9000+id);
	client.Connect ();
    client.Receive ( (char*) & mSize, lg );
    if ( lg == sizeof ( VideoSize ) )
    {
        mCurrentFrame = Mat (mSize.rows, mSize.cols, mSize.type);
    }
    while ( client.Connected () ) {
        while ( ! dataReceived ) pthread_yield ();
        dataReceived = false;
        processingData = true;
        imshow( "Frame", mCurrentFrame );
        char c=(char)waitKey(25);
        processingData = false;
        if ( c == 27 ) break;
	}
	client.Disconnect ();
}
