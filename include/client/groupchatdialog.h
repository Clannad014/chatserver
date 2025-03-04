#ifndef GROUPCHATDIALOG_H
#define GROUPCHATDIALOG_H

#include <QDialog>
#include "chatdialog.h"
#include "mylistitem.h"
#include <QListWidgetItem>
#include <QThread>
namespace Ui {
class GroupChatDialog;
}

class GroupChatDialog : public ChatDialog
{
    Q_OBJECT

public:
    explicit GroupChatDialog(QWidget *parent = nullptr,int toid = 0,QString name = "",int chattype = GROUP_CHAT_MSG);
    void displayMsg(json js);
    void updateGroupList();
    void initUi();
    void setAvatarImgVec(vector<QImage> &_avatarImgVec){this->_avatarImgVec = _avatarImgVec;}
    ~GroupChatDialog();

private slots:
    void on_sendLineEdit_returnPressed();
    void on_pushButton_clicked();

public slots:
    void reloadMembers();
private:
    Ui::GroupChatDialog *ui;
    vector<QImage> _avatarImgVec;
};

#endif // GROUPCHATDIALOG_H
