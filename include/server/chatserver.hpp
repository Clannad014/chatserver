#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "json.hpp"
// #include <muduo/net/TcpServer.h>
// #include <muduo/net/EventLoop.h>
#include "tcpcenter.h"
#include "Address.h"
#include "TcpConnectionPtr.h"
#include "chatservice.hpp"
using namespace std;
// using namespace muduo;
// using namespace muduo::net;
using json = nlohmann::json;

class ChatServer{
    public:
        ChatServer(EventLoop* loop,
            Address& listenAddr);

        void start();
    private:
        void onConnection(TcpConnectionPtr&);
        void onMessage(TcpConnectionPtr&,
                            buffer*);
        TcpSer* _server;
        EventLoop *_loop;
};

#endif
