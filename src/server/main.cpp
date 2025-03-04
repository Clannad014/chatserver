#include "chatserver.hpp"
#include "chatservice.hpp"
#include <iostream>
#include <signal.h>
using namespace std;

void resetHandler(int)
{
    ChatService::instance()->reset();
    exit(0);
}

int main(int argc,char ** argv)
{
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);
    signal(SIGINT,resetHandler);
    EventLoop loop;
    Address addr(ip,port);
    ChatServer server(&loop,addr);
    server.start();
    loop.loop();
    return 0;
}
