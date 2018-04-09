#ifndef WSSERVER_H
#define WSSERVER_H
#include "ws/server.h"

using namespace std;

using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;

typedef struct {
    WsServer::Endpoint ep;
    std::vector<shared_ptr<WsServer::Connection>> connected;
} Endpoint;

typedef struct {
    Endpoint cam0;
    Endpoint cam1;
} CamWsStreamer;

extern void InitWS(WsServer* server);

#endif
