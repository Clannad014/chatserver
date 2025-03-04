#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H

#include "user.hpp"
#include "db.hpp"
#include <vector>

class FriendModel
{
public:
    //添加好友
    void insert(int userId,int friendId);
    //查询好友列表
    vector<User> query(int userId);
};


#endif