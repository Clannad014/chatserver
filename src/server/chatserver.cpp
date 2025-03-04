#include "chatserver.hpp"
#include "json.hpp"
#include "chatservice.hpp"

#include <functional>
#include <string>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

//初始化聊天服务器对象
ChatServer::ChatServer(EventLoop *loop,
                       Address& listenAddr)
    : _server(new TcpSer(loop,listenAddr)), _loop(loop)
{
    //注册链接回调
    _server->setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
    //注册消息回调
    _server->setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2));
    //设置线程数量
    _server->setthreadnum(4);
}

//启动服务
void ChatServer::start()
{
    _server->start();
}

//上报链接相关信息的回调函数
void ChatServer::onConnection(TcpConnectionPtr &conn)
{
    if(!conn.connected())
    {
        ChatService::instance()->userCloseException(conn);
        conn.shutdown();
    }
}

//上报读写时间相关信息的回调函数
void ChatServer::onMessage(TcpConnectionPtr &conn,
               buffer *buffer)
{
    string buf = buffer->getAllString();
    json js = json::parse(buf);
    auto msghandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    msghandler(conn,js);
}
