#include "wsserver.h"

void InitWS(WsServer* server)
{
  server->config.port = 8090;
  auto &cam0 = server->endpoint["^/cam0/?$"];
  cam0.on_open = [](shared_ptr<WsServer::Connection> connection) {
    cout << "Server: Opened connection " << connection.get() << endl;
  };
  // See RFC 6455 7.4.1. for status codes
  cam0.on_close = [](shared_ptr<WsServer::Connection> connection, int status, const string & /*reason*/) {
    cout << "Server: Closed connection " << connection.get() << " with status code " << status << endl;
  };
  // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
  cam0.on_error = [](shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec) {
    cout << "Server: Error in connection " << connection.get() << ". "
         << "Error: " << ec << ", error message: " << ec.message() << endl;
  };
  auto &cam1 = server->endpoint["^/cam1/?$"];
  cam1.on_open = [](shared_ptr<WsServer::Connection> connection) {
    cout << "Server: Opened connection " << connection.get() << endl;
  };
  // See RFC 6455 7.4.1. for status codes
  cam1.on_close = [](shared_ptr<WsServer::Connection> connection, int status, const string & /*reason*/) {
    cout << "Server: Closed connection " << connection.get() << " with status code " << status << endl;
  };
  // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
  cam1.on_error = [](shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec) {
    cout << "Server: Error in connection " << connection.get() << ". "
         << "Error: " << ec << ", error message: " << ec.message() << endl;
  };
}
