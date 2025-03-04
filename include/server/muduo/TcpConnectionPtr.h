#ifndef TINY_MUDUO_TCPCONNECTIONPTR_H_
#define TINY_MUDUO_TCPCONNECTIONPTR_H_
#include"Channel.h"
#include"callback.h"
#include <sys/socket.h>
#include<stdio.h>
#include"buffer.h"
#include<sys/socket.h>
#include<memory>
using namespace std;
class EventLoop;
class TcpConnectionPtr{
public:
    TcpConnectionPtr(EventLoop *loop,int connfd);
    void SetConnectionCallback(const ConnectionCallback& cb) 
    { 
        conectionCallback_ = cb;
    } 

    void SetMessageCallback(const MessageCallback& cb) 
    {
        messageCallback_ = cb;
    }
    void handleRead();

    void connectionEstablished();
    void shutdown();
    void shutdownInLoop();
    void SendInLoop(const string s);
    void Send(const string s);
    EventLoop* selfLoop(){return loop_;}
    bool connected() const {return state_==kConnected;}

private:
    enum State{kDisconnecting,kDisconntected,kConnecting,kConnected};
    int state_;
    void setState(int state){state_=state;}

    EventLoop *loop_;
    unique_ptr<Channel>channel_;
    ConnectionCallback conectionCallback_;
    MessageCallback messageCallback_;
    buffer inputBuffer_;
};
#endif