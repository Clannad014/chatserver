#include"eventloopthreadpool.h"
#include"logger.h"

eventloopthreadpool::eventloopthreadpool(EventLoop *loop):baseloop(loop),threadPool_(),loopPool_(),thread_num_(0),next_(0){}

void eventloopthreadpool::startThreadPool()
{
    LOG_DEBUG("线程池开启\n");
    for(int i=0;i<thread_num_;i++)
    {
        unique_ptr<eventloopthread>ptr(new eventloopthread());
        loopPool_.push_back(ptr->startLoop());//startLoop中开启线程，由线程创建loop后，startLoop才能返回创建好的loop。线程开启epoll_wait并阻塞
        threadPool_.push_back(move(ptr));
    }
    LOG_DEBUG("所有线程创建的loop已被添加到容器中\n");
}
EventLoop* eventloopthreadpool::nextLoop()//容器中loop为空的话返回主loop，不为空就在容器中轮询返回loop
{
    LOG_DEBUG("轮询挑选容器中的loop\n");
    EventLoop *loop=baseloop;
    if(!loopPool_.empty())
    {
        loop=loopPool_[next_];
        next_=(next_+1)%thread_num_;
    }
    return loop;
}