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
    std::string test{"<html>\
<head><title>这是我的第一个html</title></head>\
<body>\
Hello World\
</body>\
</html>\
"};
    connection.Send(test.c_str(), test.length());
    connection.Close();
}
void HttpServer::Start(int port)
{
    _server.Bind(port);
    _server.Run();
}
