#ifndef TINY_MUDUO_EVENTLOOP_H_
#define TINY_MUDUO_EVENTLOOP_H_
#include<functional>
#include<vector>
#include"epoller.h"
#include<pthread.h>
#include"Mutex.h"
#include<memory>
#include"logger.h"
#include"Channel.h"
using namespace std;
class EventLoop{
public:
    using Channels=std::vector<Channel*>;
    using BasicFunc=std::function<void()>;
    using toDoList=std::vector<BasicFunc>;
    EventLoop();

    void loop();
    void Update(Channel*channel){epoller_->update(channel);}
    bool isInLoop(){return pthread_self()==tid_;}
    void runinloop(BasicFunc Func);
    void doCallback();
    void onlyRead();
    pthread_t selfTid(){return tid_;}
    void removeChannel(Channel*channel);
private:
    unique_ptr<epoller>epoller_;
    Channels activeChannels;
    pthread_t tid_;
    toDoList toDoList_;
    Mutex mutex_;
    int wake_fd_;
    unique_ptr<Channel>wakeup_channel_;
};
#endif