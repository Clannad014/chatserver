#include"thread.h"

std::atomic_int Thread::thread_number_(0);

Thread::Thread(ThreadFunc func):tid_(-1),func_(func),latch_(1){
    setDefaultName();
}

Thread::~Thread()
{
    pthread_detach(tid_);//主线程与子线程是有关联的，当主线程走到尽头时，通过detach与子线程分离
}


void Thread::startThread()
{
    pthread_create(&tid_,NULL,work,this);
}

void Thread::setDefaultName(){
    int num;
    thread_number_++;
    num=thread_number_;
    char buff[128]={0};
    snprintf(buff,128,"Thread%d",num);
    thread_name_=buff;
}

void* Thread::work(void* arg){
    Thread* thread=static_cast<Thread*>(arg);
    thread->func_();
    return thread;
}

std::string Thread::getThreadName(){
    return thread_name_;
}