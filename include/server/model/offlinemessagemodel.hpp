#ifndef OFFLINEMESSAGEMODEL_H
#define OFFLINEMESSAGEMODEL_H

#include <vector>
#include <string>
#include "db.hpp"
using namespace std;

class OfflineMsgModel
{
public:
    //插入离线消息
    void insert(int userId,string msg);
    //删除离线消息
    void remove(int userId);
    //查询离线消息
    vector<string> query(int userId);
};


#endif