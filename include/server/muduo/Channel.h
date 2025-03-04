#ifndef TINY_MUDUO_CHANNEL_H_
#define TINY_MUDUO_CHANNEL_H_
#include<functional>
#include<sys/epoll.h>
#include<memory>
#include"callback.h"
class EventLoop;
using namespace std;
using EventCallback=function<void()>;
class Channel 
{
public:
    Channel(EventLoop *loop,int fd);
    ~Channel();

    

    /*设置回调函数*/
    void setReadCallback(EventCallback cb){readCallback=std::move(cb);}
    void setWriteCallback(EventCallback cb){writeCallback=std::move(cb);}
    void setCloseCallback(EventCallback cb){closeCallback=std::move(cb);}

    int fd() const {return fd_;}//返回fd
    int events() const {return events_;}//返回感兴趣的事件
    void set_revents(int revents) {revents_=revents;}//提供一个设置发生了的事件的接口

    /*设置感兴趣的事件*/
    void enableReading(){events_ |=EPOLLIN;update(); }
    void enableWriting(){events_ |=EPOLLOUT;update();}

    void update();//肩负epoll_ctl的功能

    void remove();

    void handleEvent();

    EventLoop* ownnerLoop(){return loop_;}


private:
    EventLoop *loop_;
    const int fd_;
    int events_;//感兴趣的事件
    int revents_;//发生的事件
    EventCallback readCallback;
    EventCallback writeCallback;
    EventCallback closeCallback;
    

};
#endif