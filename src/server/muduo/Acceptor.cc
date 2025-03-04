#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include"Channel.h"
#include"Acceptor.h"
#include"Address.h"
#include"logger.h"
acceptor::acceptor(EventLoop *loop,const Address &addr):loop_(loop),listenfd_(socket(AF_INET,SOCK_STREAM,0)),channel_(new Channel(loop,listenfd_))
{
    Bind(addr);
    channel_->setReadCallback(std::bind(&acceptor::Accept,this));
}
void acceptor::Bind(const Address &addr)
{
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=PF_INET;
    server_addr.sin_addr.s_addr=inet_addr(addr.toIp().c_str());
    server_addr.sin_port=htons(addr.toPort());
    int ret=bind(listenfd_,(sockaddr*)&server_addr,sizeof(server_addr));
    if(ret==-1)
    {
        printf("bind error!\n");
    }
}
void acceptor::Listen()
{
    int ret=listen(listenfd_,5);
    LOG_INFO("主线程正在监听");
    if(ret==-1)
    {
        printf("listen error!\n");
    }
    channel_->enableReading();
}
void acceptor::Accept()
{
    LOG_INFO("监听到有连接请求");
    struct sockaddr_in client_addr;
    socklen_t len=sizeof(client_addr);
    int connfd=accept(listenfd_,(sockaddr*)&client_addr,&len);
    newconnectionCallback(connfd);
}