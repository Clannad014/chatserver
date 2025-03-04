#include"eventloopthread.h"
#include"EventLoop.h"
#include"logger.h"

eventloopthread::eventloopthread():
    loop_(nullptr),
    thread_(std::bind(&eventloopthread::threadFunc,this)),
    mutex_(),
    cond_(mutex_)
    {}

EventLoop* eventloopthread::startLoop()
{
    thread_.startThread();
    EventLoop *loop=nullptr;
    {
        mutexLockGuard lock(mutex_);
        while(loop_==nullptr)
        {
            cond_.Wait();//自动释放锁
        }
        loop=loop_;
    }
    return loop;
}
void eventloopthread:: threadFunc()//由线程来创建loop，防止线程还未创建loop，startLoop()就返回了loop,所以使用条件变量来阻塞startLoop,等待线程创建完成后再返回创建好的loop
{
    EventLoop loop;
    LOG_DEBUG("%s在跑\n",thread_.getThreadName().data());
    {
        mutexLockGuard lock(mutex_);
        loop_=&loop;
        cond_.Signal();
    }
    loop_->loop();
}