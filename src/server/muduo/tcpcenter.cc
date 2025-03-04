#include"tcpcenter.h"
#include"Acceptor.h"
#include"TcpConnectionPtr.h"

static EventLoop* CheckLoopNotNull(EventLoop *loop)
{
    if (loop == nullptr)
    {
        LOG_FATAL("主loop为空");
    }
    return loop;
}

TcpSer::TcpSer(EventLoop *loop,Address &address)
    :loop_(CheckLoopNotNull(loop)),
    acceptor_(new acceptor(loop,address)),//创建了一个sockfd并且已经bind
    pool(new eventloopthreadpool(loop))
{
    LOG_INFO("%lu",loop->selfTid());
    acceptor_->setNewconnectionCallback(std::bind(&TcpSer::NewConnection,this,_1));
}

TcpSer::~TcpSer()
{
    delete acceptor_;
    delete pool;
}

void TcpSer::NewConnection(int connfd)
{
    EventLoop *loop=pool->nextLoop();
    TcpConnectionPtr *ptr =new TcpConnectionPtr(loop,connfd);
    ptr->SetConnectionCallback(connectionCallback_);
    ptr->SetMessageCallback(messageCallback_);
    loop->runinloop(std::bind(&TcpConnectionPtr::connectionEstablished,ptr));
}

void TcpSer::start()
{
    LOG_INFO("%d",loop_->isInLoop());
    pool->startThreadPool();
    loop_->runinloop(std::bind(&acceptor::Listen,acceptor_));
}

