#ifndef TINY_MUDUO_LATCH_H_
#define TINY_MUDUO_LATCH_H_
#include"condition.h"

class latch{
public:
    latch(int count):count_(count),mutex_(),cond_(mutex_){}
    void CountDown()
    {
        mutexLockGuard lock(mutex_);
        count_--;
        if(count_==0)
        {
            cond_.BroadCast();
        }
    }
    void Wait()
    {
        mutexLockGuard lock(mutex_);
        while(count_>0)
        {
            cond_.Wait();
        }
    }
private:
    int count_;
    Mutex mutex_;
    condition cond_;
};
#endif
