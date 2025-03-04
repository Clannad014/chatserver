#include "offlinemessagemodel.hpp"

//插入离线消息
void OfflineMsgModel::insert(int userId,string msg)
{
    // 组装sql语句
    char sql[1024 * 10] = {0};
    sprintf(sql, "insert into OfflineMessage values(%d,'%s')",userId,msg.c_str());
    MySQL mysql;
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}
//删除离线消息
void OfflineMsgModel::remove(int userId)
{
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "delete from OfflineMessage where userid=%d",userId);
    MySQL mysql;
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}
 //查询离线消息
vector<string> OfflineMsgModel::query(int userId)
{
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select message from OfflineMessage where userid=%d", userId);
    vector<string> vec;
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr)
            {
                vec.push_back(row[0]);
            }
        }
        mysql_free_result(res);
    }
    return vec;
}
