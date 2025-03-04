#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include "logger.h"
#include "TcpConnectionPtr.h"
#include "redis.hpp"
// #include <muduo/net/TcpConnection.h>
#include <functional>
#include <mutex>
#include <unordered_map>
#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
// using namespace muduo;
// using namespace muduo::net;
using namespace std;

#include "json.hpp"
using json = nlohmann::json;

using MsHandler = std::function<void(TcpConnectionPtr &conn,json &js)>;

//聊天服务器业务类
class ChatService
{
    public:
        //获取单例对象的接口函数
        static ChatService *instance();
        //处理登录业务
        void login(TcpConnectionPtr &conn,json &js);
        //处理注销业务
        void loginout(TcpConnectionPtr &conn,json &js);
        //处理注册业务
        void reg(TcpConnectionPtr &conn,json &js);
        //一对一聊天业务
        void oneChat(TcpConnectionPtr &conn,json &js);
        //添加好友业务
        void addFriend(TcpConnectionPtr &conn,json &js);

        void addFriendRes(TcpConnectionPtr &conn, json &js);
        //创建群聊业务
        void createGroup(TcpConnectionPtr &conn,json &js);
        //加入群聊业务
        void addGroup(TcpConnectionPtr &conn,json &js);
        //群组聊天业务
        void chatGroup(TcpConnectionPtr &conn,json &js);
        void updateGroupList(TcpConnectionPtr &conn,json &js);
        void updateAvatar(TcpConnectionPtr &conn,json &js);
        //处理用户的异常退出
        void userCloseException(TcpConnectionPtr &conn);
        //服务器异常退出，用户状态重置
        void reset();
        //获取消息对应的处理器
        MsHandler getHandler(int msgid);
        //从redis消息队列中获取订阅的消息
        void handlerRedisSubscribeMessage(int,string);
        //返回好友列表，已知conn
        void returnFriendList(TcpConnectionPtr &conn,int userId);
        //返回好友列表，未知conn
        void returnFriendList(int userId);
        //返回群组列表
        void returnGroupList(TcpConnectionPtr &conn,int userId);
        void returnStateMsg(int id,int toid,string state);
    private:
        ChatService();
        //存储消息id及其对应的业务处理方法
        unordered_map<int,MsHandler> _msgHandlerMap;
        mutex _connMutex;
        unordered_map<int,TcpConnectionPtr*> _userConnMap;
        UserModel _userModel;
        OfflineMsgModel _offlineMsgModel;
        FriendModel _friendModel;
        GroupModel _groupModel;
        Redis _redis;
};


#endif
