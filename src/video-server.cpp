#include <iostream>
#include <map>
#include <chrono>
#include <thread>

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#include "process/ThreadedProcess.h"

#include "tcp/TcpServer.h"
#include "tcp/TcpRegisteredClient.h"

typedef struct {
    int cols;
    int rows;
    int type;
    int depth;
    int nChannels;
    int step;
} VideoSize;

/*
VideoCapture mCamera;
Mat mCurrentFrame;
VideoSize mSize;
*/
class VideoCaptureServer;

class VideoServer : public TcpServer
{
public:
    VideoCaptureServer* mData;
	VideoServer(VideoCaptureServer* data,int port);
	int OnConnect(TcpRegisteredClient* client);
	void OnDisconnect(TcpRegisteredClient* client);
	void OnReceive(TcpRegisteredClient* client,char* message, int length);
};

class VideoCaptureServer : public ThreadedProcess
{
public:
    VideoServer mServer;
    VideoCapture mCapture;
    Mat mFrame;
    VideoSize mSize;
    int mVideoID;
	VideoCaptureServer(int videoID,int port) : mServer(this,port), mVideoID ( videoID )
    {
    }
	virtual int threadStartFunction ()
    {
        mCapture = VideoCapture(mVideoID);
        mCapture.set(CV_CAP_PROP_FRAME_WIDTH,800);
        mCapture.set(CV_CAP_PROP_FRAME_HEIGHT,448);
        mCapture >> mFrame;
        mSize.cols = mCapture.get(CV_CAP_PROP_FRAME_WIDTH); //mFrame.cols;
        mSize.rows = mCapture.get(CV_CAP_PROP_FRAME_HEIGHT); // mFrame.rows;
        mSize.type = mFrame.type ();
        mSize.depth = 8;//mFrame.depth ();
        mSize.nChannels = mFrame.channels ();
        mSize.step = mFrame.step1 ();
        mServer.Reserve ();
        mServer.Start ();
        return 0;
    }
	virtual int threadInnerFunction ()
    {
        mCapture >> mFrame;
        imshow( "Frame", mFrame );
        char c=(char)waitKey(25);
        return 0;
    }
	virtual int threadStopFunction ()
    {
        mServer.Stop ();
        mServer.Release ();
        return 0;
    }
};

VideoServer::VideoServer(VideoCaptureServer* data,int port) : TcpServer(port), mData ( data ) {}
int VideoServer::OnConnect(TcpRegisteredClient* client)
{
    client->Send ( (char*) & mData->mSize, sizeof ( VideoSize ) );
    return 0;
}
void VideoServer::OnDisconnect(TcpRegisteredClient* client)
{
}
void VideoServer::OnReceive(TcpRegisteredClient* client,char* message, int length)
{
    int value = *( (int*) message );
    if ( value == 1 )
    {
        client->Send ( (char*) mData->mFrame.ptr (), mData->mSize.cols * mData->mSize.rows * ( mData->mSize.depth / 8 ) * mData->mSize.nChannels );
    }
}

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
    int id = atoi ( argv[1] );
    VideoCaptureServer server0(id,9000+id);
	server0.start ();
	while ( ! done ) {
        if (HasInput())
        {
            std::string message;
            cin >> message;
            if ( message == "quit" ) break;
        }
		server0.mServer.Update ();
	}
	server0.stop ();
}
