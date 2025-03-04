#ifndef TINY_MUDUO_EVENTLOOPTHREAD_H_
#define TINY_MUDUO_EVENTLOOPTHREAD_H_
#include"thread.h"
class EventLoop;
class eventloopthread{
public:
    eventloopthread();
    EventLoop* startLoop();
    void threadFunc();
private:
    EventLoop *loop_;
    Thread thread_;
    Mutex mutex_;
    condition cond_;
};

#endif