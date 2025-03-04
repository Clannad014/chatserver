#include"TimeStamp.h"

TimeStamp::TimeStamp():time_(0){}

TimeStamp::TimeStamp(time_t time):time_(time){}

TimeStamp TimeStamp::now(){
    return TimeStamp(time(NULL));//time(NULL)返回的是一个距离1970年的时间，用该时间来构造TimeStamp并且返回
}

std::string TimeStamp::now_time_string(){
    struct tm *time_now=localtime(&time_);
    char buff[128]={0};
    snprintf(buff,128,"%4d/%02d/%02d  %02d:%02d:%02d",
    time_now->tm_year+1900,
    time_now->tm_mon+1,
    time_now->tm_mday,
    time_now->tm_hour,
    time_now->tm_min,
    time_now->tm_sec);
    return buff;
}
// #include<iostream>
// int main(){
//     std::cout<<TimeStamp::now().now_time_string()<<std::endl;
// }