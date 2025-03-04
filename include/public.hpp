#ifndef PUBLIC_H
#define PUBLIC_H

/*
    client和server的公共文件
*/
enum EnMsgType{
    LOGIN_MSG = 1,//登录消息
    LOGIN_MSG_ACK,//登录应答消息
    LOGIN_OUT_MSG,
    STATE_UPDATE_ACK,
    REG_MSG ,//注册消息
    REG_MSG_ACK ,//注册应答消息
    ONE_CHAT_MSG,//发送聊天消息
    ADD_FRIEND_MSG,//添加好友消息
    ADD_FRIEND_ACK,//添加好友消息ACK
    RETURN_FRIEND_ACK,//返回好友列表
    CREATE_GROUP_MSG,//创建群聊消息
    RETURN_GROUP_ACK,//返回群组消息ACK
    UPDATE_GROUP_MSG,//返回群组消息
    ADD_GROUP_MSG,//加入群聊消息
    ADD_GROUP_ACK,//加入群聊消息ACK
    GROUP_CHAT_MSG,//群聊天消息
    UPDATE_AVATAR_MSG,
};

#endif
