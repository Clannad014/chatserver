#include "friendnoticeitem.h"
#include "ui_friendnoticeitem.h"
#include "mainmenu.h"
extern MyTcpSocket *Socket;//全局变量，socket对象

FriendNoticeItem::FriendNoticeItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendNoticeItem)
{
    ui->setupUi(this);
}



FriendNoticeItem::~FriendNoticeItem()
{
    delete ui;
}

void FriendNoticeItem::setName(QString name)
{
    //设置用户名称
    ui->nameLabel->setText(name);
}

void FriendNoticeItem::setAvatar(int avatarIndex,QImage &avatar)
{
    //设置用户头像
    this->avatarIndex = avatarIndex;
    ui->avatarWidget->setImage(avatar);
}

void FriendNoticeItem::addFriendRes(bool ack)
{
    //向服务器发送好友添加的回复请求
    json resonse;
    resonse["id"] = id;
    resonse["friendid"] = friendid;
    resonse["msgid"] = ADD_FRIEND_ACK;
    if(ack == true)
    {
        resonse["add_result"] = "yes";
    }
    else
    {
        resonse["add_result"] = "no";
    }
    Socket->Send(QString(resonse.dump().c_str()).toUtf8());
    //请求内容已被处理，删除item
    emit haveProcess(index);
}


void FriendNoticeItem::on_agreePushButton_clicked()
{
    //同意添加好友
    addFriendRes(true);
}


void FriendNoticeItem::on_refusePushButton_clicked()
{
    //拒绝添加好友
    addFriendRes(false);
}

