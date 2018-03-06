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

typedef struct {
    Mat mFrame;
    VideoSize mSize;
    int mLength;
} VideoCaptureData;


class VideoCaptureServer;

/**
 *
 */

class VideoServer : public TcpServer
{
public:
    VideoCaptureData* mData;
	VideoServer(VideoCaptureData* data,int port);
	int OnConnect(TcpRegisteredClient* client);
	void OnDisconnect(TcpRegisteredClient* client);
	void OnReceive(TcpRegisteredClient* client,char* message, int length);
};

/**
 *
 */

class VideoBcastServer : public TcpServer
{
public:
    VideoCaptureData* mData;
	VideoBcastServer(VideoCaptureData* data,int port);
	int OnConnect(TcpRegisteredClient* client);
	void OnDisconnect(TcpRegisteredClient* client);
	void OnReceive(TcpRegisteredClient* client,char* message, int length);
};

/**
 *
 */

class VideoCaptureServer : public ThreadedProcess
{
public:
    VideoCapture mCapture;
    VideoServer* mServer[3];
    VideoBcastServer* mBcastServer[3];
    VideoCaptureData* mCaptureData[3];
    Mat mFrame;
    VideoSize mSize;
    int mVideoID;
    int mPort;
	VideoCaptureServer(int videoID,int port) : mVideoID ( videoID ), mPort(port)
    {
    }
	virtual int threadStartFunction ()
    {
        mCapture = VideoCapture(mVideoID);
        mCapture.set(CV_CAP_PROP_FRAME_WIDTH,800);
        mCapture.set(CV_CAP_PROP_FRAME_HEIGHT,448);
        for ( int i = 0 ; i < 3 ; i ++ )
        {
            mCaptureData[i] = new VideoCaptureData;
            mCaptureData[i]->mFrame.create(448/pow(2,i),800/pow(2,i),CV_8UC3);
            mCaptureData[i]->mSize.cols = mCaptureData[i]->mFrame.cols;
            mCaptureData[i]->mSize.rows = mCaptureData[i]->mFrame.rows;
            mCaptureData[i]->mSize.type = mCaptureData[i]->mFrame.type ();
            mCaptureData[i]->mSize.depth = 8;//mFrame.depth ();
            mCaptureData[i]->mSize.nChannels = mCaptureData[i]->mFrame.channels ();
            mCaptureData[i]->mSize.step = mCaptureData[i]->mFrame.step1 ();
            mCaptureData[i]->mLength = mCaptureData[i]->mSize.cols * mCaptureData[i]->mSize.rows * ( mCaptureData[i]->mSize.depth / 8 ) * mCaptureData[i]->mSize.nChannels;
        }
        mCapture >> mFrame;
        mSize.cols = mCapture.get(CV_CAP_PROP_FRAME_WIDTH); //mFrame.cols;
        mSize.rows = mCapture.get(CV_CAP_PROP_FRAME_HEIGHT); // mFrame.rows;
        mSize.type = mFrame.type ();
        mSize.depth = 8;//mFrame.depth ();
        mSize.nChannels = mFrame.channels ();
        mSize.step = mFrame.step1 ();
        for ( int i = 0 ; i < 3 ; i ++ )
        {
            mServer[i] = new VideoServer ( mCaptureData[i], mPort + i );
            mBcastServer[i] = new VideoBcastServer ( mCaptureData[i], mPort + 10 + i );
        }
        for ( int i = 0 ; i < 3 ; i ++ )
        {
            mServer[i]->Reserve ();
            mBcastServer[i]->Reserve ();
            mServer[i]->Start ();
            mBcastServer[i]->Start ();
        }
        return 0;
    }
	virtual int threadInnerFunction ()
    {
        mCapture >> mFrame;
        for ( int i = 0 ; i < 3 ; i ++ )
            resize ( mFrame, mCaptureData[i]->mFrame, mCaptureData[i]->mFrame.size () );
        for ( int i = 0 ; i < 3 ; i ++ )
        {
            mBcastServer[i]->Send( (char*) mCaptureData[i]->mFrame.ptr (), mCaptureData[i]->mLength );
        }
        imshow( "Frame", mFrame );
        char c=(char)waitKey(25);
        return 0;
    }
	virtual int threadStopFunction ()
    {
        for ( int i = 0 ; i < 3 ; i ++ )
        {
            mBcastServer[i]->Stop ();
            mServer[i]->Stop ();
            mBcastServer[i]->Release ();
            mServer[i]->Release ();
        }
        return 0;
    }
	virtual int Update ()
    {
        /*
        for ( int i = 0 ; i < 3 ; i ++ )
        {
            std::cout << __LINE__ << std::endl;
            mBcastServer[i]->Update ();
            std::cout << __LINE__ << std::endl;
            mServer[i]->Update ();
            std::cout << __LINE__ << std::endl;
        }
        */
        return 0;
    }
};

/**
 *
 */

VideoServer::VideoServer(VideoCaptureData* data,int port) : TcpServer(port), mData ( data ) {}
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
        client->Send ( (char*) mData->mFrame.ptr (), mData->mLength );
    }
}

/**
 *
 */

VideoBcastServer::VideoBcastServer(VideoCaptureData* data,int port) : TcpServer(port,true), mData ( data ) {}
int VideoBcastServer::OnConnect(TcpRegisteredClient* client)
{
    client->Send ( (char*) & mData->mSize, sizeof ( VideoSize ) );
    return 0;
}
void VideoBcastServer::OnDisconnect(TcpRegisteredClient* client)
{
}
void VideoBcastServer::OnReceive(TcpRegisteredClient* client,char* message, int length)
{
}

/**
 *
 */

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
    VideoCaptureServer server0(id,9000+id*100);
	server0.start ();
	while ( ! done ) {
        if (HasInput())
        {
            std::string message;
            cin >> message;
            if ( message == "quit" ) break;
        }
		server0.Update ();
	}
	server0.stop ();
}
