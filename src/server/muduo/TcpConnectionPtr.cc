#include"TcpConnectionPtr.h"
#include <unistd.h>
#include"logger.h"
#include "EventLoop.h"

TcpConnectionPtr:: TcpConnectionPtr(EventLoop *loop,int connfd):
    loop_(loop),
    channel_(new Channel(loop,connfd)),
    state_(kDisconnecting)
{
    channel_->setReadCallback(std::bind(&TcpConnectionPtr::handleRead,this));
}

void TcpConnectionPtr::handleRead()
{
    int ret=inputBuffer_.Read(channel_->fd());
    if(ret>0)
    {
        messageCallback_(*this,&inputBuffer_);
    }
    else if(ret==0)
    {
        setState(kDisconntected);
        LOG_DEBUG("客户%d断开连接\n",channel_->fd());
        conectionCallback_(*this);
    }
    else
    {
        printf("recv error!\n");
    }
}

void TcpConnectionPtr::SendInLoop(const string s)
{
    int ret=send(channel_->fd(),s.data(),s.size(),0);
    if(ret<0)
    {
        printf("send error!\n");
    }
}
void TcpConnectionPtr::Send(const string s)
{
    loop_->runinloop(bind(&TcpConnectionPtr::SendInLoop,this,s));
}

void TcpConnectionPtr::connectionEstablished()
{
    setState(kConnected);
    channel_->enableReading();
    conectionCallback_(*this);
}

void TcpConnectionPtr::shutdown() {
    loop_->runinloop(bind(&TcpConnectionPtr::shutdownInLoop,this));
}

void TcpConnectionPtr::shutdownInLoop(){
    close(channel_->fd());
}