#include"EventLoop.h"
#include"Channel.h"
#include<stdio.h>
#include <sys/eventfd.h>
#include <unistd.h>

EventLoop::EventLoop():
    epoller_(new epoller()),
    tid_(pthread_self()),
    wake_fd_(eventfd(0, EFD_NONBLOCK)),
    wakeup_channel_(new Channel(this,wake_fd_))
    {
        LOG_INFO("%lu",tid_);
        wakeup_channel_->setReadCallback(std::bind(&EventLoop::onlyRead,this));
        wakeup_channel_->enableReading();
    }

void EventLoop::loop()
{
    while(1)
    {
        int number=epoller_->EpollWait();
        epoller_->fillActiveChannels(number,activeChannels);
        for(auto &channel:activeChannels)
        {
            channel->handleEvent();
        }
        activeChannels.clear();
        doCallback();
    }
}
void EventLoop::runinloop(BasicFunc Func)
{
    if(isInLoop())
    {
        Func();
    }
    else
    {
        {
            mutexLockGuard lock(mutex_);
            toDoList_.emplace_back(Func);
        }
        uint64_t write_one_byte = 1;  
        write(wake_fd_, &write_one_byte, sizeof(write_one_byte));//唤醒正在阻塞的loop处理用户要求的回调事件
    }
}
void EventLoop::doCallback()
{
    toDoList functors;
    {
        mutexLockGuard lock(mutex_);
        functors=move(toDoList_);
    }
    for(auto func:functors)
    {
        func();
    }
}
void EventLoop::onlyRead()
{
    uint64_t read_one_byte = 1;
    read(wake_fd_, &read_one_byte, sizeof(read_one_byte));
}

void EventLoop::removeChannel(Channel*channel){
    epoller_->delChannel(channel);
}