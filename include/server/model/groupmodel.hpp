#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include "group.hpp"
#include "db.hpp"
#include<string>
#include <vector>
using namespace std;

class GroupModel
{
public:
    //创建群聊业务
    bool creatGroup(Group &group);
    //加入群聊业务
    bool addGroup(int groupId,int userId,string role);
    //查询该用户的所有群聊
    vector<Group> queryGroups(int userId);
    //查询该群聊里除了自己的所有用户id
    vector<int> queryGroupUsers(int userId,int groupId);
    bool hasGroup(int groupId);
};


#endif
