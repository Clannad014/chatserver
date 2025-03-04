#include "groupmodel.hpp"
#include <iostream>

// 创建群聊业务
bool GroupModel::creatGroup(Group &group)
{

    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into AllGroup(groupname,groupdesc) values('%s','%s')", group.getGroupName().c_str(), group.getGroupDesc().c_str());
    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            // 获取插入成功的用户数据生成的主键
            group.setGroupId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}

// 加入群聊业务
bool GroupModel::addGroup(int groupId, int userId, string role)
{
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into GroupUser values(%d,%d,'%s')", groupId, userId, role.c_str());
    MySQL mysql;
    if (mysql.connect())
    {
        return mysql.update(sql);
    }
    return false;
}
// 查询该用户的所有群聊
vector<Group> GroupModel::queryGroups(int userId)
{
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select a.id,a.groupname,a.groupdesc from AllGroup a inner join GroupUser b on b.groupid=a.id where b.userid=%d", userId);
    vector<Group> vec;
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                Group group;
                group.setGroupId(atoi(row[0]));
                group.setGroupName(row[1]);
                group.setGroupDesc(row[2]);
                vec.push_back(group);
            }
        }
        mysql_free_result(res);
    }

    for (Group &group : vec)
    {
        sprintf(sql, "select a.id,a.name,a.state,b.grouprole,a.avatarIndex from User a inner join GroupUser b on a.id=b.userid where b.groupid=%d", group.getGroupId());
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                GroupUser user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                user.setGroupRole(row[3]);
                user.setAvatar(atoi(row[4]));
                group.getGroupUsers().push_back(user);
            }
        }
        mysql_free_result(res);
    }
    return vec;
}
// 查询该群聊里除了自己的所有用户id
vector<int> GroupModel::queryGroupUsers(int userId, int groupId)
{
    char sql[1024] = {0};
    sprintf(sql, "select userid from GroupUser where userid!=%d and groupid=%d", userId, groupId);
    vector<int> vec;
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {

                vec.push_back(atoi(row[0]));
            }
        }
        mysql_free_result(res);
    }
    return vec;
}

bool GroupModel::hasGroup(int groupId)
{
    char sql[1024] = {0};
    sprintf(sql, "select id from AllGroup where id=%d",groupId);
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if(row != nullptr)
            {
                mysql_free_result(res);
                return true;
            }
        }
        mysql_free_result(res);
    }
    return false;
}
