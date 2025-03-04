#ifndef LOGGER_H_
#define LOGGER_H_
#include<string>
#define LOG_INFO(format, ...)\
    do\
    {\
        logger logger_=logger::get_single_instance();\
        logger_.set_level(INFO);\
        char buff[128]={0};\
        snprintf(buff, 128 , format ,  ##__VA_ARGS__);\
        logger_.log(buff);\
    } while(0)

#define LOG_ERROR(format, ...)\
    do\
    {\
        logger logger_=logger::get_single_instance();\
        logger_.set_level(ERROR);\
        char buff[128]={0};\
        snprintf(buff, 128 , format ,  ##__VA_ARGS__);\
        logger_.log(buff);\
    } while(0)

#define LOG_FATAL(format, ...)\
    do\
    {\
        logger logger_=logger::get_single_instance();\
        logger_.set_level(FATAL);\
        char buff[128]={0};\
        snprintf(buff, 128 , format ,  ##__VA_ARGS__);\
        logger_.log(buff);\
    } while(0)

#define LOG_DEBUG(format, ...)\
do\
{\
    logger logger_=logger::get_single_instance();\
    logger_.set_level(DEBUG);\
    char buff[128]={0};\
    snprintf(buff, 128 , format , ##__VA_ARGS__);\
    logger_.log(buff);\
} while(0)

enum log_level{INFO,ERROR,FATAL,DEBUG};

class logger{
public:
    static logger& get_single_instance();
    void set_level(int log_level);
    void log(std::string msg);
private:
    int log_level_;
};
#endif