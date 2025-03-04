#include "groupchatdialog.h"
#include "ui_groupchatdialog.h"
#include "mainmenu.h"

extern User g_currentUser;//全局变量，保存当前登录用户的信息
extern vector<User> g_currentUserFriendList;//全局变量，保存当前登录用户的好友列表信息
extern vector<Group> g_currentUserGroupList;//全局变量，保存当前登录用户的群组列表信息
extern MyTcpSocket *Socket;//全局变量，socket对象
extern vector<QString> _avatarVec;//全局变量，保存系统头像的图片路径

GroupChatDialog::GroupChatDialog(QWidget *parent,int toid,QString name,int chattype) :
    ChatDialog(parent,toid,name,chattype),
    ui(new Ui::GroupChatDialog)
{
    ui->setupUi(this);
    //聊天标题设置
    ui->nameLabel->setText(name);
    ui->nameLabel->setAlignment(Qt::AlignCenter);
    //创建滚动条
    QtMaterialScrollBar*mscrollBar = new QtMaterialScrollBar(this);
    mscrollBar->setMinimumHeight(8);
    ui->memberListWidget->setVerticalScrollBar(mscrollBar);
    connect(ui->MsgScrollArea->verticalScrollBar(),&QScrollBar::rangeChanged,[this](){
        ui->MsgScrollArea->verticalScrollBar()->setValue(ui->MsgScrollArea->verticalScrollBar()->maximum());
    });//当向页面中新加入消息时，滚动条自动滚动到最底部
}

GroupChatDialog::~GroupChatDialog()
{
    delete ui;
}

void GroupChatDialog::displayMsg(json js)
{

    QString message (js["msg"].get<string>().c_str());
    RecvBox * recvbox = new RecvBox(ui->MsgScrollArea);//创建消息对象
    recvbox->setAvatar(_avatarVec[js["avatar"].get<int>()]);//添加头像
    recvbox->setName(js["name"].get<string>().c_str());//设置消息发送者姓名
    recvbox->hide();//设置消息对象不可见，防止加入页面时触发页面重绘机制，导致页面闪烁
    ui->scrollAreaWidgetContents->layout()->addWidget(recvbox);
    recvbox->setText(message);//设置消息内容
    recvbox->show();//当完全加入页面后再显示，这样触发一次页面重绘，防止页面闪烁
}

void GroupChatDialog::initUi()
{
    ui->setupUi(this);
}

void GroupChatDialog::on_sendLineEdit_returnPressed()
{
    //向服务器发送消息发送请求
    QString message = ui->sendLineEdit->text();
    json js;
    js["msgid"] = chattype;
    js["id"] = g_currentUser.getId();
    js["name"] = g_currentUser.getName();
    js["groupId"] = toid;
    js["groupName"] = name.toStdString();
    js["msg"] = message.toStdString();
    js["time"] = MainMenu::getCurrentTime();
    js["avatar"] = g_currentUser.getAvatar();
    string buffer = js.dump();
    Socket->Send(QString(buffer.c_str()).toUtf8());
    ui->sendLineEdit->clear();
    //将发送的消息显示到自己的页面上
    SendBox * sendbox = new SendBox(ui->MsgScrollArea);
    sendbox->setAvatar(_avatarVec[g_currentUser.getAvatar()]);
    sendbox->setName(g_currentUser.getName().c_str());
    sendbox->hide();
    ui->scrollAreaWidgetContents->layout()->addWidget(sendbox);
    sendbox->setText(message);
    sendbox->show();
}

void GroupChatDialog::reloadMembers()
{
    //重新加载群成员列表
    ui->memberListWidget->clear();
    for(int i = 0;i < g_currentUserGroupList.size();i++)
    {
        if(toid == g_currentUserGroupList[i].getGroupId())
        {
            int j = 0;
            for(GroupUser &user : g_currentUserGroupList[i].getGroupUsers())
            {
                QListWidgetItem *item = new QListWidgetItem(ui->memberListWidget);
                item->setSizeHint(QSize(120,80));
                MyListItem *myItem = new MyListItem(this);
                myItem->setAvatar(user.getAvatar(),_avatarImgVec[user.getAvatar()]);
                myItem->setName(user.getName().c_str());
                myItem->setId(user.getId());
                myItem->setState(user.getState().c_str());
                myItem->setType(MENUITEM);
                ui->memberListWidget->addItem(item);
                ui->memberListWidget->setItemWidget(item,myItem);
            }
        }
    }
}


void GroupChatDialog::on_pushButton_clicked()
{
    //点击刷新按钮，可手动刷新
    this->updateGroupList();
}


void GroupChatDialog::updateGroupList()
{
    //向服务器发送群成员更新请求
    json js;
    js["msgid"] = UPDATE_GROUP_MSG;
    js["id"] = g_currentUser.getId();
    std::string request = js.dump();
    Socket->Send(QString(request.c_str()).toUtf8());
    //重新加载群成员列表
    reloadMembers();
}


