#ifndef LIST_CLOCKER_H
#define LIST_CLOCKER_H
#include<functional>
#include<time.h>
using namespace std;
using countDownCallBack=function<void()>;

class clocker{
public:
    void setCountDownCallBack(countDownCallBack cb){countDownCallBack_=cb;}
    //超时的时间节点(绝对时间)
    time_t expire;
    clocker* prev;
    clocker* next;
    void doNow(){countDownCallBack_();}
    private:
    countDownCallBack countDownCallBack_;
};

class clocker_lst{
public:
    clocker_lst();
    ~clocker_lst();
    //插入链表当中
    void add_clocker(clocker* object);

    void adjust_clocker(clocker* object);

    void del_clocker(clocker* object);

    void tick();
private:
    //插入链表头之后
    void add_clocker(clocker* object,clocker* head);

    clocker* head;
    clocker* tail;
};

#endif