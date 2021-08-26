#include "HttpServer.h"
#include <functional>
using namespace squid;
HttpServer::HttpServer(int threadCount) : _server(threadCount)
{
    _server.RegisterMessageReceiveEvent(
        std::bind(&HttpServer::OnMessageReceive, this, std::placeholders::_1, std::placeholders::_2));
}
void HttpServer::OnMessageReceive(Connection &connection, BufStream &stream)
{
    std::string test{"HTTP/1.1 400 Bad Request\r\n\r\n"};
    connection.Send(test.c_str(), test.length());
    connection.Close();
}
void HttpServer::Start(int port)
{
    _server.Bind(port);
    _server.Run();
}
