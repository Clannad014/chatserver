#ifndef TINY_MUDUO_CONDITION_H_
#define TINY_MUDUO_CONDITION_H_
#include"Mutex.h"

class condition
{
public:
    condition(Mutex& Mutex_):mutex_(Mutex_)
    {
        pthread_cond_init(&cond_,NULL);
    }
    ~condition()
    {
        pthread_cond_destroy(&cond_);
    }
    bool Wait()
    {
        int ret=pthread_cond_wait(&cond_,mutex_.self());//加入到等待队列必须先上锁
        return ret==0;
    }
    bool Signal()
    {
        return pthread_cond_signal(&cond_)==0;
    }
    bool BroadCast()
    {
        return pthread_cond_broadcast(&cond_)==0;
    }
private:
    Mutex& mutex_;
    pthread_cond_t cond_;
};
#endif