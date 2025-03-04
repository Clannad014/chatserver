#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_
#include<time.h>
#include<string>
class TimeStamp{
public:
    TimeStamp();
    TimeStamp(time_t time);
    static TimeStamp now();
    std::string now_time_string();
private:
    time_t time_;
};
#endif