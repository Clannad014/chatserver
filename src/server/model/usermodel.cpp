#include "usermodel.hpp"
#include "db.hpp"
#include <iostream>
using namespace std;

// User表的增加方法
bool UserModel::insert(User &user)
{
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into User(name,password,state,avatarIndex) values('%s','%s','%s',%d)",
            user.getName().c_str(), user.getPassword().c_str(), user.getState().c_str(),user.getAvatar());
    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            // 获取插入成功的用户数据生成的主键
            user.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}

User UserModel::query(int id)
{
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select * from User where id=%d", id);
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row != nullptr)
            {
                User user;
                user.setId(id);
                user.setName(row[1]);
                user.setPassword(row[2]);
                user.setState(row[3]);
                user.setAvatar(atoi(row[4]));
                mysql_free_result(res);
                return user;
            }
        }
    }
    return User();
}

bool UserModel::updateState(User user)
{
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "update User set state='%s' where id=%d",user.getState().c_str(),user.getId());
    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}

void UserModel::resetState()
{
    // 组装sql语句
    char sql[1024] = "update User set state='offline' where state='online'";
    MySQL mysql;
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}

bool UserModel::updateAvatar(User user)
{
    char sql[1024] = {0};
    sprintf(sql, "update User set avatarIndex=%d where id=%d",user.getAvatar(),user.getId());
    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}
