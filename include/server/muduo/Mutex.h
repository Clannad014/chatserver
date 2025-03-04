#ifndef TINY_MUDUO_MUTEX_H_
#define TINY_MUDUO_MUTEX_H_
#include<pthread.h>
class Mutex{
public:
    Mutex()
    {
        pthread_mutex_init(&mutex_,NULL);
    }
    ~Mutex()
    {
        pthread_mutex_destroy(&mutex_);
    }
    bool Lock()
    {
        return pthread_mutex_lock(&mutex_)==0;
    }
    bool Unlook()
    {
        return pthread_mutex_unlock(&mutex_)==0;
    }
    pthread_mutex_t* self(){return &mutex_;}
private:
    pthread_mutex_t mutex_;
};

class  mutexLockGuard//创建即加锁，析构即解锁。但是锁资源不归该类管
{
public:
    mutexLockGuard(Mutex& Mutex_):mutex_(Mutex_)
    {
        mutex_.Lock();
    }
    ~mutexLockGuard()
    {
        mutex_.Unlook();
    }
    
private:
    Mutex& mutex_;
};
#endif
