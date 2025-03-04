#ifndef TINY_MUDUO_THREAD_H_
#define TINY_MUDUO_THREAD_H_
#include"latch.h"
#include<functional>
#include<string>
#include<atomic>
#include<memory>

class Thread{
public:
    using ThreadFunc=std::function<void()>;
    Thread(ThreadFunc func);
    ~Thread();
    void startThread();
    void setDefaultName();
    static void* work(void* arg);
    std::string getThreadName();
private:
    pthread_t tid_;
    ThreadFunc func_;
    std::string thread_name_;
    latch latch_;
    static std::atomic_int thread_number_;
};
#endif