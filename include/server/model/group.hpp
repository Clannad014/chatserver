#ifndef GROUP_H
#define GROUP_H

#include "groupuser.hpp"
#include <vector>
#include <string>
using namespace std;

class Group
{
public:
    Group(int groupId,string groupName,string groupDesc){this->groupId = groupId;this->groupName = groupName;this->groupDesc = groupDesc;}
    Group(){};
    void setGroupId(int groupId){this->groupId = groupId;}
    void setGroupName(string groupName){this->groupName = groupName;}
    void setGroupDesc(string groupDesc){this->groupDesc = groupDesc;}
    void setUserVec(vector<GroupUser> &userVec){this->userVec = userVec;}
    int getGroupId(){return this->groupId;}
    string getGroupName(){return this->groupName;}
    string getGroupDesc(){return this->groupDesc;}
    vector<GroupUser> &getGroupUsers(){return this->userVec;}
private:
    int groupId;
    string groupName;
    string groupDesc;
    vector<GroupUser> userVec;
};

#endif