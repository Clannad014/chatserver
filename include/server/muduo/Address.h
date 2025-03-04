#ifndef TINY_MUDUO_ADDRESS_H_
#define TINY_MUDUO_ADDRESS_H_
#include <string>
#include <netinet/in.h>
using namespace std;

class Address
{
public:
    Address(string ip,int port):ip_(ip),port_(port){}
    std::string toIp() const
    {
        return ip_;
    }
    int toPort() const
    {
        return port_;
    }

private:
    std::string ip_;
    int port_;
};
#endif