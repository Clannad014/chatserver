#include"logger.h"
#include"TimeStamp.h"
#include<iostream>

logger& logger::get_single_instance(){
    static logger log;
    return log;
}

void logger::set_level(int log_level){
    log_level_=log_level;
}

void logger::log(std::string msg){
    switch(log_level_){
        case INFO:
            std::cout<<"[INFO] ";
            break;
        case ERROR:
            std::cout<<"[ERROR] ";
            break;
        case FATAL:
            std::cout<<"[FATAL] ";
            break;
        case DEBUG:
            std::cout<<"[DEBUG] ";
            break;
        default:
            break;
    }
    std::cout<<TimeStamp::now().now_time_string()<<" : "<<msg<<std::endl;
}