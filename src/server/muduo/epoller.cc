#include"epoller.h"
#include"Channel.h"
#include <unistd.h>
epoller::epoller():epollfd_(epoll_create(8)),events_(8){}

epoller::~epoller(){close(epollfd_);}

int epoller::setnonblocking(int fd)
        {
            int old_option=fcntl(fd,F_GETFL);
            int new_option=old_option | O_NONBLOCK;
            fcntl(fd,F_SETFL,new_option);
            return old_option;
        }

void epoller::update(Channel *channel)
{
    if(channel->events() & EPOLLIN)
    {
        updateChannel(EPOLL_CTL_ADD,channel);
        setnonblocking(channel->fd());
    }
    else if(channel->events() & EPOLLRDHUP)
    {
        updateChannel(EPOLL_CTL_DEL,channel);
    }
    else
    {

    }
}
void epoller::fillActiveChannels(int num,Channels &activeChannels)
{
    for(int i=0;i<num;i++)
    {
        Channel *ptr=static_cast<Channel*>(events_[i].data.ptr);
        ptr->set_revents(events_[i].events);
        activeChannels.emplace_back(ptr);
    }
}
void epoller::updateChannel(int op,Channel *channel)//将channel中fd感兴趣的事件注册到epoller中的epollfd中去
{
    struct epoll_event event;
    memset(&event,0,sizeof(event));
    event.events=channel->events();
    event.data.fd=channel->fd();
    event.data.ptr=static_cast<void*>(channel);
    epoll_ctl(epollfd_,op,channel->fd(),&event);
}

void epoller::delChannel(Channel*channel){
    updateChannel(EPOLL_CTL_DEL,channel);
}

