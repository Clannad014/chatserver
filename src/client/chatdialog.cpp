#include "chatdialog.h"
#include "ui_chatdialog.h"
#include "mainmenu.h"

extern User g_currentUser;
extern vector<User> g_currentUserFriendList;
extern vector<Group> g_currentUserGroupList;
extern int clientfd;

ChatDialog::ChatDialog(QWidget *parent,int toid,QString name,int chattype) :
    QDialog(parent),
    ui(new Ui::ChatDialog)
{
    this->toid = toid;
    this->chattype = chattype;
    this->name = name;
}

ChatDialog::~ChatDialog()
{
    emit closeWindow(toid,chattype);
    delete ui;
}

void ChatDialog::closeEvent(QCloseEvent *e)
{
    e->ignore();
    this->hide();
}

int ChatDialog::getId()
{
    return toid;
}

int ChatDialog::getType()
{
    return chattype;
}
void ChatDialog::initUi()
{
    ui->setupUi(this);
}
