#include "chatservice.hpp"
#include "public.hpp"
#include "group.hpp"
#include <string>
// #include <muduo/base/Logging.h>
#include <iostream>
// using namespace muduo;

// 获取单例对象的接口函数
ChatService *ChatService::instance()
{
    static ChatService service;
    return &service;
}

// 注册消息及其对应的handler回调操作
ChatService::ChatService()
{
    _msgHandlerMap.insert({LOGIN_MSG, std::bind(&ChatService::login, this, _1, _2)});
    _msgHandlerMap.insert({LOGIN_OUT_MSG, std::bind(&ChatService::loginout, this, _1, _2)});
    _msgHandlerMap.insert({REG_MSG, std::bind(&ChatService::reg, this, _1, _2)});
    _msgHandlerMap.insert({ONE_CHAT_MSG, std::bind(&ChatService::oneChat, this, _1, _2)});
    _msgHandlerMap.insert({ADD_FRIEND_MSG, std::bind(&ChatService::addFriend, this, _1, _2)});
    _msgHandlerMap.insert({ADD_FRIEND_ACK, std::bind(&ChatService::addFriendRes, this, _1, _2)});
    _msgHandlerMap.insert({CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this, _1, _2)});
    _msgHandlerMap.insert({ADD_GROUP_MSG, std::bind(&ChatService::addGroup, this, _1, _2)});
    _msgHandlerMap.insert({GROUP_CHAT_MSG, std::bind(&ChatService::chatGroup, this, _1, _2)});
    _msgHandlerMap.insert({UPDATE_GROUP_MSG, std::bind(&ChatService::updateGroupList, this, _1, _2)});
    _msgHandlerMap.insert({UPDATE_AVATAR_MSG, std::bind(&ChatService::updateAvatar, this, _1, _2)});
    // 连接redis服务器
    if (_redis.connect())
    {
        // 设置上报消息的回调
        _redis.init_notify_handler(std::bind(&ChatService::handlerRedisSubscribeMessage, this, _1, _2));
    }
}

// 服务器异常退出，重置用户状态
void ChatService::reset()
{
    _userModel.resetState();
}
// 获取消息对应的处理器
MsHandler ChatService::getHandler(int msgid)
{
    return _msgHandlerMap[msgid];
}

// 处理登录业务
void ChatService::login(TcpConnectionPtr &conn, json &js)
{
    int id = js["id"].get<int>();
    string pwd = js["password"];
    User user = _userModel.query(id);
    if (id == user.getId() && pwd == user.getPassword())
    {
        // 该用户已经登录，不允许重复登录
        if (user.getState() == "online")
        {
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 2;
            response["id"] = user.getId();
            response["errmsg"] = "The account has already been logged in";
            conn.Send(response.dump());
        }
        else
        {
            // 登录成功，记录用户连接信息
            {
                lock_guard<mutex> lock(_connMutex);
                _userConnMap.insert({id, &conn});
            }

            // id用户登录成功，向redis订阅channel(id)
            _redis.subscribe(id);
            // 登录成功,更新用户状态信息，state offline=>online
            user.setState("online");
            _userModel.updateState(user);
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 0;
            response["id"] = user.getId();
            response["name"] = user.getName();
            response["avatar"] = user.getAvatar();
            // 查询该用户是否有离线消息
            vector<string> vec = _offlineMsgModel.query(id);
            if (!vec.empty())
            {
                response["offlinemsg"] = vec;
                _offlineMsgModel.remove(id);
            }
            // 显示该用户的好友列表
            vector<string> vec2;
            vector<User> uservec = _friendModel.query(id);
            if (!uservec.empty())
            {
                for (User &u : uservec)
                {
                    json js;
                    js["id"] = u.getId();
                    js["name"] = u.getName();
                    js["state"] = u.getState();
                    js["avatar"] = u.getAvatar();
                    vec2.push_back(js.dump());
                    //通知好友列表该用户上线，并返回更新后的好友列表
                    returnStateMsg(id,u.getId(),"online");
                }

            }
            response["friends"] = vec2;
            // 显示该用户的群组列表
            // vector<string> vec3;
            vector<string> grpstr;
            vector<Group> groupvec = _groupModel.queryGroups(id);
            if (!groupvec.empty())
            {
                for (Group &group : groupvec)
                {
                    json grpjson;
                    grpjson["id"] = group.getGroupId();
                    grpjson["groupname"] = group.getGroupName();
                    grpjson["groupdesc"] = group.getGroupDesc();
                    vector<string> userstr;
                    for (GroupUser &groupuser : group.getGroupUsers())
                    {
                        json js;
                        js["id"] = groupuser.getId();
                        // std::cout << groupuser.getId();
                        js["name"] = groupuser.getName();
                        js["state"] = groupuser.getState();
                        js["role"] = groupuser.getGroupRole();
                        js["avatar"] = groupuser.getAvatar();
                        userstr.push_back(js.dump());
                    }
                    grpjson["users"] = userstr;
                    grpstr.push_back(grpjson.dump());
                }

            }
            response["groups"] = grpstr;
            conn.Send(response.dump());
        }
    }
    else
    {
        // 该用户不存在，存在但密码错误，登录失败
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;
        response["errmsg"] = "Wrong username or password";
        conn.Send(response.dump());
    }
}
// 处理注册业务
void ChatService::reg(TcpConnectionPtr &conn, json &js)
{
    string name = js["name"];
    string password = js["password"];
    User user;
    user.setName(name);
    user.setPassword(password);
    bool state = _userModel.insert(user);
    if (state)
    {
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 0;
        response["id"] = user.getId();
        conn.Send(response.dump());
        LOG_INFO("注册成功");
    }
    else
    {
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;
        response["id"] = user.getId();
        conn.Send(response.dump());
        LOG_INFO("注册失败");
    }
}
void ChatService::loginout(TcpConnectionPtr &conn, json &js)
{
    int userid = js["id"].get<int>();
    {
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnMap.find(userid);
        if (it != _userConnMap.end())
        {
            _userConnMap.erase(it);
        }

    }

    // 用户注销，相当于下线，在redis中取消订阅通道
    _redis.unsubscribe(userid);
    User user(userid, "", "", "offline");
    _userModel.updateState(user);
    vector<User> friendlist = _friendModel.query(userid);
    for(User &u : friendlist)
    {
        returnStateMsg(userid,u.getId(),"offline");
    }
}
// 处理用户的异常退出
void ChatService::userCloseException(TcpConnectionPtr &conn)
{
    User user;
    {
        lock_guard<mutex> lock(_connMutex);
        for (auto it = _userConnMap.begin(); it != _userConnMap.end(); it++)
        {
            if (it->second == &conn)
            {
                user.setId(it->first);
                _userConnMap.erase(it);
                break;
            }
        }
    }

    // 用户注销，相当于下线，在redis中取消订阅通道
    _redis.unsubscribe(user.getId());

    if (user.getId() != -1)
    {
        user.setState("offline");
        _userModel.updateState(user);
        vector<User> vec = _friendModel.query(user.getId());
        for (User &u : vec)
        {
            //通知好友列表该用户下线，并返回更新后的好友列表
            returnStateMsg(user.getId(),u.getId(),"offline");
        }
    }
}
// 一对一聊天业务
void ChatService::oneChat(TcpConnectionPtr &conn, json &js)
{
    int toid = js["toid"].get<int>();
    {
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnMap.find(toid);
        if (it != _userConnMap.end())
        {
            // toid在线，转发消息，服务器主动推送消息给toid用户
            it->second->Send(js.dump());
            return;
        }
    }
    // 查询是否在线
    User user = _userModel.query(toid);
    if (user.getState() == "online")
    {
        _redis.publish(toid, js.dump());
        return;
    }
    // toid不在线，存储离线消息
    if(js["msgid"].get<int>() == ONE_CHAT_MSG || js["msgid"].get<int>() == GROUP_CHAT_MSG)
    {
        _offlineMsgModel.insert(toid, js.dump());
    }
}
// 添加好友业务
void ChatService::addFriend(TcpConnectionPtr &conn, json &js)
{
    int userId = js["id"].get<int>();
    int friendId = js["friendId"].get<int>();
    User user = _userModel.query(friendId);
    if(user.getId() == -1)
    {
        json ack;
        ack["msgid"] = ADD_FRIEND_ACK;
        ack["msg"] = "the id of user doesn't exist!";
        ack["errno"] = 1;
        string buffer = ack.dump();
        conn.Send(buffer);
        return;
    }
    json response;
    response["msgid"] = ADD_FRIEND_MSG;
    response["id"] = userId;
    response["toid"] = friendId;
    response["avatar"] = js["avatar"].get<int>();
    response["name"] = js["name"];
    oneChat(conn,response);
}

void ChatService::addFriendRes(TcpConnectionPtr &conn, json &js)
{
    int userid = js["id"].get<int>();
    int friendid = js["friendid"].get<int>();
    json response;
    response["msgid"] = ADD_FRIEND_ACK;
    response["toid"] = userid;
    response["friendid"] = friendid;
    string result = js["add_result"];
    if(result == "yes")
    {
        response["errno"] = 0;
        _friendModel.insert(userid,friendid);
        _friendModel.insert(friendid,userid);
        returnFriendList(userid);
        returnFriendList(conn,friendid);
        response["msg"] = "add friend success !";
    }
    else if(result == "no")
    {
        response["errno"] = 0;
        response["msg"] = "your request was denied";
    }
    oneChat(conn,response);
}

// 创建群聊业务
void ChatService::createGroup(TcpConnectionPtr &conn, json &js)
{
    int userId = js["id"].get<int>();
    string groupName = js["groupName"];
    string groupDesc = js["groupDesc"];
    Group group(-1, groupName, groupDesc);
    if (_groupModel.creatGroup(group))
    {
        _groupModel.addGroup(group.getGroupId(), userId, "creator");
    }
    returnGroupList(conn, userId);
}
// 加入群聊业务
void ChatService::addGroup(TcpConnectionPtr &conn, json &js)
{
    int userId = js["id"].get<int>();
    int groupId = js["groupId"].get<int>();
    if(!_groupModel.hasGroup(groupId))
    {
        json ack;
        ack["msgid"] = ADD_GROUP_ACK;
        ack["msg"] = "the id of group doesn't exist!";
        ack["errno"] = 1;
        string buffer = ack.dump();
        conn.Send(buffer);
        return;
    }
    if(_groupModel.addGroup(groupId, userId, "normal"))
    {
//        returnGroupList(conn, userId);
        json ack;
        ack["msgid"] = ADD_GROUP_ACK;
        ack["msg"] = "add group success !";
        ack["errno"] = 0;
        string buffer = ack.dump();
        conn.Send(buffer);
    }
    else
    {
        json ack;
        ack["msgid"] = ADD_GROUP_ACK;
        ack["msg"] = "add group fail !";
        ack["errno"] = 1;
        string buffer = ack.dump();
        conn.Send(buffer);
    }
}
// 群组聊天业务
void ChatService::chatGroup(TcpConnectionPtr &conn, json &js)
{
    int userId = js["id"].get<int>();
    int groupId = js["groupId"].get<int>();
    vector<int> groupUsers = _groupModel.queryGroupUsers(userId, groupId);
    lock_guard<mutex> lock(_connMutex);
    for (int id : groupUsers)
    {
        auto it = _userConnMap.find(id);
        if (it != _userConnMap.end())
        {
            // 在线转发消息
            it->second->Send(js.dump());
        }
        else
        {
            // 查询是否在线
            User user = _userModel.query(id);
            if (user.getState() == "online")
            {
                _redis.publish(id, js.dump());
                continue;
            }
            // 存储离线消息
            _offlineMsgModel.insert(id, js.dump());
        }
    }
}

void ChatService::updateGroupList(TcpConnectionPtr &conn, json &js)
{
    int userId = js["id"].get<int>();
    returnGroupList(conn,userId);
}

void ChatService::updateAvatar(TcpConnectionPtr &conn, json &js)
{
    int userId = js["id"].get<int>();
    int avatarIndex = js["avatar"].get<int>();
    _userModel.updateAvatar(User(userId,"","","",avatarIndex));
}

void ChatService::handlerRedisSubscribeMessage(int userid, string message)
{
    lock_guard<mutex> lock(_connMutex);
    auto it = _userConnMap.find(userid);
    if (it != _userConnMap.end())
    {
        it->second->Send(message);
        return;
    }
    json js = json::parse(message.c_str());
    if (js["msgid"].get<int>() == ONE_CHAT_MSG || js["msgid"].get<int>() == GROUP_CHAT_MSG)
    {
        // 存储该用户的离线消息
        _offlineMsgModel.insert(userid, message);
    }
}

void ChatService::returnFriendList(TcpConnectionPtr &conn, int userId)
{
    json response;
    response["msgid"] = RETURN_FRIEND_ACK;
    response["id"] = userId;
    vector<string> vec1;
    vector<User> uservec1 = _friendModel.query(userId);
    if (!uservec1.empty())
    {
        for (User &user : uservec1)
        {
            json js;
            js["id"] = user.getId();
            js["name"] = user.getName();
            js["state"] = user.getState();
            js["avatar"] = user.getAvatar();
            vec1.push_back(js.dump());
        }
        response["friends"] = vec1;
    }
    conn.Send(response.dump());
}

void ChatService::returnGroupList(TcpConnectionPtr &conn, int userId)
{
    json response;
    response["msgid"] = RETURN_GROUP_ACK;
    response["id"] = userId;
    vector<string> grpstr;
    vector<Group> groupvec = _groupModel.queryGroups(userId);
    if (!groupvec.empty())
    {
        for (Group &group : groupvec)
        {
            json grpjson;
            grpjson["id"] = group.getGroupId();
            grpjson["groupname"] = group.getGroupName();
            grpjson["groupdesc"] = group.getGroupDesc();
            vector<string> userstr;
            for (GroupUser &groupuser : group.getGroupUsers())
            {
                json js;
                js["id"] = groupuser.getId();
                js["name"] = groupuser.getName();
                js["state"] = groupuser.getState();
                js["role"] = groupuser.getGroupRole();
                js["avatar"] = groupuser.getAvatar();
                userstr.push_back(js.dump());
            }
            grpjson["users"] = userstr;
            grpstr.push_back(grpjson.dump());
        }
        response["groups"] = grpstr;
    }
    conn.Send(response.dump());
}


void ChatService::returnFriendList(int userId)
{
    User u = _userModel.query(userId);
    if (u.getState() == "online")
    {
        //用户和好友在同一台服务器上
        {
            lock_guard<mutex> lock(_connMutex);
            auto it = _userConnMap.find(u.getId());
            if (it != _userConnMap.end())
            {
                returnFriendList(*(it->second), u.getId());
                return;
            }
        }
        //不在同一台服务器上
        json response;
        response["msgid"] = RETURN_FRIEND_ACK;
        response["id"] = u.getId();
        vector<string> vec2;
        vector<User> uservec2 = _friendModel.query(u.getId());
        if (!uservec2.empty())
        {
            for (User &user : uservec2)
            {
                json js;
                js["id"] = user.getId();
                js["name"] = user.getName();
                js["state"] = user.getState();
                js["avatar"] = user.getAvatar();
                vec2.push_back(js.dump());
            }
            response["friends"] = vec2;
        }
        _redis.publish(u.getId(), response.dump());
    }
}

void ChatService::returnStateMsg(int id,int toid,string state)
{
    //用户和好友在同一台服务器上
    User u = _userModel.query(toid);
    if(u.getState() == "online")
    {
        {
            lock_guard<mutex> lock(_connMutex);
            auto it = _userConnMap.find(u.getId());
            if (it != _userConnMap.end())
            {
                json response;
                response["msgid"] = STATE_UPDATE_ACK;
                response["id"] = id;
                response["state"] = state;
                response["toid"] = toid;
                it->second->Send(response.dump());
                return;
            }
        }
    }
    //不在同一台服务器上
    json response;
    response["msgid"] = STATE_UPDATE_ACK;
    response["id"] = id;
    response["state"] = state;
    response["toid"] = toid;
    _redis.publish(toid,response.dump());
}


