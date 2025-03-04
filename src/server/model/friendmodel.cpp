#include "friendmodel.hpp"


//插入好友记录
void FriendModel::insert(int userId,int friendId)
{
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into Friend values(%d,%d)",userId,friendId);
    MySQL mysql;
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}
//查询好友记录
vector<User> FriendModel::query(int userId)
{
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select a.id,a.name,a.state,a.avatarIndex from User a inner join Friend b on b.friendid=a.id where b.userid=%d", userId);
    vector<User> vec;
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr)
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                user.setAvatar(atoi(row[3]));
                vec.push_back(user);
            }
        }
        mysql_free_result(res);
    }
    return vec;
}
