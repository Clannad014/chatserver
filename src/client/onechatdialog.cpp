#include "onechatdialog.h"
#include "ui_onechatdialog.h"
#include "mainmenu.h"

extern User g_currentUser;//全局变量，保存当前登录用户的信息
extern vector<User> g_currentUserFriendList;//全局变量，保存当前登录用户的好友列表信息
extern vector<Group> g_currentUserGroupList;//全局变量，保存当前登录用户的群组列表信息
extern MyTcpSocket *Socket;//全局变量，socket对象
extern vector<QString> _avatarVec;//全局变量，保存系统头像的图片路径

OneChatDialog::OneChatDialog(QWidget *parent,int toid,QString name,int chattype) :
    ChatDialog(parent,toid,name,chattype),
    ui(new Ui::OneChatDialog)
{
    ui->setupUi(this);
    //聊天标题设置
    ui->nameLabel->setText(name);
    ui->nameLabel->setAlignment(Qt::AlignCenter);
    //创建滚动条
    QtMaterialScrollBar *mscrollBar = new QtMaterialScrollBar(this);
    mscrollBar->setMinimumWidth(5);
    ui->MsgScrollArea->setVerticalScrollBar(mscrollBar);
    connect(ui->MsgScrollArea->verticalScrollBar(),&QScrollBar::rangeChanged,[this](){
        ui->MsgScrollArea->verticalScrollBar()->setValue(ui->MsgScrollArea->verticalScrollBar()->maximum());
    });//当向页面中新加入消息时，滚动条自动滚动到最底部
}

OneChatDialog::~OneChatDialog()
{
    delete ui;
}


void OneChatDialog::displayMsg(json js)
{
    QString message (js["msg"].get<string>().c_str());
    RecvBox * recvbox = new RecvBox(this);//创建消息对象
    recvbox->hideNameLabel();//隐藏发送者姓名的ui部分
    recvbox->setAvatar(_avatarVec[js["avatar"].get<int>()]);//添加头像
    recvbox->hide();//设置消息对象不可见，防止加入页面时触发页面重绘机制，导致页面闪烁
    ui->scrollAreaWidgetContents->layout()->addWidget(recvbox);
    recvbox->setText(message);//设置消息内容
    recvbox->show();//当完全加入页面后再显示，这样触发一次页面重绘，防止页面闪烁
}
void OneChatDialog::initUi()
{
    ui->setupUi(this);
}

void OneChatDialog::on_sendLineEdit_returnPressed()
{
    //向服务器发送消息发送请求
    QString message = ui->sendLineEdit->text();
    json js;
    js["msgid"] = chattype;
    js["id"] = g_currentUser.getId();
    js["name"] = g_currentUser.getName();
    js["toid"] = toid;
    js["msg"] = message.toStdString();
    js["time"] = MainMenu::getCurrentTime();
    js["avatar"] = g_currentUser.getAvatar();
    string buffer = js.dump();
    Socket->Send(QString(buffer.c_str()).toUtf8());
    ui->sendLineEdit->clear();
    //将发送的消息显示到自己的页面上
    SendBox * sendbox = new SendBox(this);
    sendbox->hideNameLabel();
    sendbox->setAvatar(_avatarVec[g_currentUser.getAvatar()]);
    sendbox->hide();
    ui->scrollAreaWidgetContents->layout()->addWidget(sendbox);
    sendbox->setText(message);
    sendbox->show();
}


