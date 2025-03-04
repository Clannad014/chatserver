#ifndef TINY_MUDUO_TCPSERVER_H_
#define TINY_MUDUO_TCPSERVER_H_
#include"EventLoop.h"
#include"Acceptor.h"
#include"callback.h"
#include"eventloopthreadpool.h"
class Address;
class TcpSer{
public:
    TcpSer(EventLoop *loop,Address &address);
    ~TcpSer();
    void start();

    void setConnectionCallback(ConnectionCallback cb){connectionCallback_=std::move(cb);}
    void setMessageCallback(MessageCallback cb){messageCallback_=std::move(cb);}
    void NewConnection(int connfd);
    void setthreadnum(int num){pool->setnum(num);}
private:
    EventLoop *loop_;
    acceptor *acceptor_;
    eventloopthreadpool* pool;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
};
#endif