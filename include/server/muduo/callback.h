#ifndef TINY_MUDUO_CALLBACK_H_
#define TINY_MUDUO_CALLBACK_H_
#include<functional>
#include"buffer.h"
/*回调函数类型*/
class TcpConnectionPtr;

using CloseCallback=std::function<void(TcpConnectionPtr&)>;
using ConnectionCallback=std::function<void(TcpConnectionPtr&)>;
using MessageCallback=std::function<void(TcpConnectionPtr&,buffer*)>;

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
#endif