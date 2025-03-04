#include"Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop *loop,int fd):loop_(loop),fd_(fd){}

Channel::~Channel(){}


void Channel::update()//从loop中调用epoller来epoll_ctl感兴趣事件
{
    loop_->Update(this);
}

void Channel::handleEvent()
{
    if(revents_ & EPOLLIN )
    {
        if(readCallback)
        {
            readCallback();
        }
    }
    else if(revents_ & EPOLLOUT)
    {
        if(writeCallback)
        {
            writeCallback();
        }
    }
    else 
    {

    }
}

void Channel::remove(){
    loop_->removeChannel(this);
}