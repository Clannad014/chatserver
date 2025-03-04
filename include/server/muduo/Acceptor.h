#ifndef TINY_MUDUO_ACCEPTOR_H_
#define TINY_MUDUO_ACCEPTOR_H_
#include<functional>

class EventLoop;
class Address;
class Channel;

class acceptor{
public:
    using NewconnectionCallback=std::function<void(int)>;
    acceptor(EventLoop *loop,const Address &addr);
    void Bind(const Address &addr);
    void Listen();
    void Accept();

    void setNewconnectionCallback(NewconnectionCallback cb)
    {
        newconnectionCallback=cb;
    }
private:
    EventLoop *loop_;
    int listenfd_;
    Channel *channel_;
    NewconnectionCallback newconnectionCallback;
};
#endif