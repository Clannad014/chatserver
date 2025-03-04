#ifndef EVENTLOOPTHREADPOOL_H_
#define EVENTLOOPTHREADPOOL_H_
#include"eventloopthread.h"
#include<vector>
#include<memory>
using namespace std;
class eventloopthreadpool{
public:
    eventloopthreadpool(EventLoop *loop);
    void startThreadPool();
    EventLoop* nextLoop();
    void setnum(int num){thread_num_=num;}
private:
    EventLoop *baseloop;
    vector<unique_ptr<eventloopthread>> threadPool_;
    vector<EventLoop*> loopPool_;
    int thread_num_;
    int next_;//轮询获得loop用
};
#endif