#ifndef TINY_MUDUO_EPOLLER_H_
#define TINY_MUDUO_EPOLLER_H_
#include <fcntl.h> 
#include<sys/epoll.h>
#include<vector>
#include<sys/epoll.h>
#include <string.h>
class Channel;
class epoller{
public:
    using Events=std::vector<epoll_event>;
    using Channels=std::vector<Channel *>;
    epoller();
    ~epoller();
    int setnonblocking(int fd);
    int EpollWait(){return epoll_wait(epollfd_,&*events_.begin(),8,-1);}
    void update(Channel *channel);
    void fillActiveChannels(int num,Channels &activeChannels);
    void updateChannel(int op,Channel *channel);
    void delChannel(Channel*channel);
private:
    int epollfd_;
    Events events_;//通过epoll_wait获得的发生事件
};
#endif