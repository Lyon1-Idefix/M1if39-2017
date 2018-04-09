#include "webserver.h"
#include "wsserver.h"

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

HttpServer webserver;

int main() {
    bool done = false;
    InitWeb (&webserver);
    thread server_thread([&webserver]() {
        std::cout << "start server" << std::endl;
        webserver.start();
    });
    while ( ! done ) {
        if ( HasInput () ) {
            std::string command;
            std::cin >> command;
            if ( command == "quit" ) done = true;
        }
    }
    webserver.stop ();
}
