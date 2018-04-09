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

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_RBUTTONDOWN )
     {
          cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_MBUTTONDOWN )
     {
          cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if ( event == EVENT_MOUSEMOVE )
     {
          cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;

     }
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
Mat empty_image = Mat::zeros(mCurrentFrame.rows, mCurrentFrame.cols, CV_8UC1);
Mat result_blue(mCurrentFrame.rows, mCurrentFrame.cols, CV_8UC3); // notice the 3 channels here!
Mat result_green(mCurrentFrame.rows, mCurrentFrame.cols, CV_8UC3); // notice the 3 channels here!
Mat result_red(mCurrentFrame.rows, mCurrentFrame.cols, CV_8UC3); // notice the 3 channels here!
    }
     namedWindow("blue channel", 1);
     setMouseCallback("blue channel", CallBackFunc, NULL);
    while ( client.Connected () ) {
        while ( ! dataReceived ) pthread_yield ();
        dataReceived = false;
        processingData = true;
        vector<Mat> spl;
split(mCurrentFrame,spl);

// Create an zero pixel image for filling purposes - will become clear later
// Also create container images for B G R channels as colour images
Mat empty_image = Mat::zeros(mCurrentFrame.rows, mCurrentFrame.cols, CV_8UC1);
Mat result_blue(mCurrentFrame.rows, mCurrentFrame.cols, CV_8UC3); // notice the 3 channels here!
Mat result_green(mCurrentFrame.rows, mCurrentFrame.cols, CV_8UC3); // notice the 3 channels here!
Mat result_red(mCurrentFrame.rows, mCurrentFrame.cols, CV_8UC3); // notice the 3 channels here!

// Create blue channel
Mat in1[] = { spl[0], empty_image, empty_image };
int from_to1[] = { 0,0, 1,1, 2,2 };
mixChannels( in1, 3, &result_blue, 1, from_to1, 3 );

// Create green channel
Mat in2[] = { empty_image, spl[1], empty_image };
int from_to2[] = { 0,0, 1,1, 2,2 };
mixChannels( in2, 3, &result_green, 1, from_to2, 3 );

// Create red channel
Mat in3[] = { empty_image, empty_image, spl[2]};
int from_to3[] = { 0,0, 1,1, 2,2 };
mixChannels( in3, 3, &result_red, 1, from_to3, 3 );

imshow("blue channel",result_blue);
imshow("green channel",result_green);
imshow("red channel",result_red);
        imshow( "Frame", mCurrentFrame );
        char c=(char)waitKey(25);
        processingData = false;
        if ( c == 27 ) break;
	}
	client.Disconnect ();
}
